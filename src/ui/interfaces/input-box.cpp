#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap InputBox::styles({
    {"header", Style(201, std::nullopt, {SGR::Bold})},
    {"box", Styles::faint},
});
// clang-format on

Text InputBox::control_strip = Text("[ESC] close").style(Styles::faint);

InputBox::InputBox(const termui::string &field, const termui::string &response, const termui::string &placeholder)
  : Interface(TermSetups::fullscreen_with_cursor),
    field(field),
    response(response),
    placeholder(placeholder),
    header(field, styles.at("header")),
    input_field(response, placeholder),
    box(0, 0, Borders::rounded, styles.at("box")) {}

void InputBox::show() {
  term_setup.configure();

  state = State::CONTINUE;
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

void InputBox::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.write(terminal.height(), 2, control_strip.render()); // print ctl strip first to avoid interfering

  uint origin_row = (terminal.height() - static_cast<uint>(h)) / 2, origin_col = (terminal.width() - static_cast<uint>(w)) / 2;

  terminal.write(origin_row, origin_col, box.render());
  terminal.write(origin_row + 1, origin_col + 2, header.render());
  terminal.write(origin_row + 3, origin_col + 2, input_field.render());
  terminal.flush();
}

void InputBox::process_input() {
  std::string ec = terminal.read();

  if (ec == key::ESC) { // escape to close
    state = State::EXIT;

  } else if (ec == key::DEL) { // remove last char
    std::string &r = response;
    if (r.size()) {
      r = std::string(r.begin(), r.end() - 1);
    }
    reprint = true;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // accept basically all chars
    std::string &r = response;
    r += ec;
    reprint = true;
  }
};

void InputBox::update_size() {
  w = 0.25 * terminal.width();
  h = 6;

  header.width(w - 4).height(1);
  input_field.width(w - 4).height(2);
  box.resize(w, h);
  control_strip.width(terminal.width() - 2).height(1);
}

} // namespace termui
