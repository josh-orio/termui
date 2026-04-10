#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap InfoBox::styles({
  {"title", Style(57, std::nullopt, {SGR::Bold})},
  {"button", Style(std::nullopt, 212)},
  {"box", Styles::faint}
});
// clang-format on

Text InfoBox::control_banner("[ESC] close  [↵] close", Styles::faint);

InfoBox::InfoBox(const termui::string &t, const termui::string &c)
  : Interface(TermSetups::fullscreen),
    title(t),
    content(c),
    box(0, 0, Borders::rounded, styles.at("box")),
    header(title, styles.at("title")),
    body(content),
    close("Close", styles.at("button")) {}

void InfoBox::show() {
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

void InfoBox::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  uint origin_row = (termui::terminal.height() - h) / 2, origin_col = (termui::terminal.width() - w) / 2;

  terminal.write(origin_row, origin_col, box.render());
  terminal.write(origin_row + 1, origin_col + 2, header.render());
  terminal.write(origin_row + 3, origin_col + 2, body.render());
  terminal.write(origin_row + 8, origin_col + (w - 2 - 8), close.render());

  // auto term_h = terminal.height(), term_w = terminal.width();

  // terminal.write((term_h - h) / 2, (term_w - w) / 2, box.render());
  // terminal.CursorUp(h - 2);
  // terminal.CursorBack(w - 2);

  // terminal.write(header.render());
  // terminal.CursorDown(2);
  // // terminal.CursorBack(header.width());
  // terminal.

  // terminal.write(body.render());
  // terminal.CursorDown(2);
  // terminal.CursorBack(close.width());

  // terminal.write(close.render());

  terminal.write(terminal.height(), 2, control_banner.render());
  terminal.flush();
}

void InfoBox::process_input() {
  std::string ec = terminal.read();

  if (ec == key::ESC) { // ESC closes info box
    state = State::EXIT;

  } else if (ec == key::ENTER) {
    state = State::EXIT;

  } else {
    state = State::CONTINUE;
  }
}

void InfoBox::update_size() {
  w = 0.25 * terminal.width();
  h = 10;

  box.resize(w, h);
  header.width(w - 4).height(1);
  body.width(w - 4).height(4);
  close.width(8).focus();
  control_banner.width(terminal.width() - 2).height(1);
}

} // namespace termui
