#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap InputPage::styles({
  {"header", Style(212, std::nullopt, {SGR::Bold})},
  {"focus", Style(212, std::nullopt, {SGR::Bold})}
});
// clang-format on

Text InputPage::control_strip("[ESC] close  [↵] close", Styles::faint);

InputPage::InputPage(const termui::string &title, const termui::strings &fields, const termui::strings &responses, uint ls)
  : Interface(TermSetups::fullscreen),
    title(title),
    fields(fields),
    responses(responses),
    header(title, styles.at("header")),
    cursor(0),
    start_line(0),
    line_seperation(ls) {}

void InputPage::show() {
  term_setup.configure();

  state = State::CONTINUE;
  selected = false;
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();

  } while (state == State::CONTINUE);

  term_setup.reset();
}

void InputPage::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.write(2, 2, header.render());

  for (int i = start_line; i < std::min(fields.size(), static_cast<size_t>(start_line + visible_lines)); i++) {
    std::string &f = fields.at(i);

    Input iput = Input(responses.at(i), "Start typing...").width(terminal.width() - f.length() - 7 /* some horizonstal overhead */).height(1);

    if (i == cursor) {
      terminal.write(4 + ((i - start_line) * (line_seperation + 1)), 2, renderer.render("> " + f, styles.at("focus")) + ": " + iput.render());
    } else {
      terminal.write(4 + ((i - start_line) * (line_seperation + 1)), 4, f + ": " + iput.render());
    }
  }

  terminal.write(terminal.height(), 2, control_strip.render());

  // move the cursor into position and adjust cursor toggle as needed
  if (selected) {
    std::string &f = fields.at(cursor), &r = responses.at(cursor);

    terminal.write(4 + ((cursor - start_line) * (line_seperation + 1)),                              // top overhead + (indicated row * line seperation)
                   std::min(static_cast<size_t>(terminal.width() - 2), 6 + f.length() + r.length()), // tracks end column of response without allowing overflow
                   "");

    terminal.ShowCursor();
  } else {
    terminal.HideCursor();
  }

  terminal.flush();
}

void InputPage::process_input() {
  std::string ec = terminal.read();

  if (ec == key::ENTER) { // select/deselect
    selected = !selected; // flip
    reprint = true;

  } else if (ec == key::ESC) { // escape to close
    if (!selected) {           // dont close if field selected
      state = State::EXIT;
    }

  } else if (ec == key::U_ARROW) {
    if (!selected) {
      cursor -= (cursor > 0) ? 1 : 0; // decrement but dont let (cursor < 0)
      start_line -= (cursor < start_line) ? 1 : 0;
    }
    reprint = true;

  } else if (ec == key::D_ARROW) {
    if (!selected) {
      cursor += (cursor < (int)fields.size() - 1) ? 1 : 0; // increment but dont let (cursor > fields.size)
      start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
    }
    reprint = true;

  } else if (MouseInteraction(ec).match(EventType::ScrollUp)) {
    if (!selected) {
      cursor -= (cursor > 0) ? 1 : 0; // decrement but dont let (cursor < 0)
      start_line -= (cursor < start_line) ? 1 : 0;
    }
    reprint = true;

  } else if (MouseInteraction(ec).match(EventType::ScrollDown)) {
    if (!selected) {
      cursor += (cursor < (int)fields.size() - 1) ? 1 : 0; // increment but dont let (cursor > fields.size)
      start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
    }
    reprint = true;

  } else if (ec == key::DEL) { // remove last char
    if (selected) {
      std::string &r = responses.at(cursor);

      if (r.size()) {
        r = std::string(r.begin(), r.end() - 1);
      }
    }
    reprint = true;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // accept basically all chars
    if (selected) {
      std::string &r = responses.at(cursor);

      r += ec;
    }
    reprint = true;
  }
};

void InputPage::update_size() {
  header.width(terminal.width() - voh).height(1);
  control_strip.width(terminal.width() - voh).height(1);

  visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_lines + 1) + (line_seperation * visible_lines);

    if (space_used > (terminal.height() - voh)) {
      break;
    }
    visible_lines++;
  }
}

} // namespace termui
