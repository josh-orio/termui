#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap InfoBox::styles({
  {"title", Style(57, Color::Inherit(), {SGR::Bold})},
  {"button", Style(Color::Inherit(), 212)},
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

  state   = State::Continue;
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();

  } while (state == State::Continue);

  term_setup.reset();
}

void InfoBox::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  unsigned int origin_row = (termui::terminal.height() - h) / 2, origin_col = (termui::terminal.width() - w) / 2;

  terminal.MoveCursor(origin_row, origin_col);
  box.render();

  terminal.MoveCursor(origin_row + 1, origin_col + 2);
  header.render();

  terminal.MoveCursor(origin_row + 3, origin_col + 2);
  body.render();

  terminal.MoveCursor(origin_row + 8, origin_col + (w - 2 - 8));
  close.render();

  terminal.MoveCursor(terminal.height(), 2);
  control_banner.render();

  terminal.flush();
}

void InfoBox::process_input() {
  std::string ec = terminal.read();

  if (ec == key::ESC) { // ESC closes info box
    state = State::Exit;
  }
  else if (ec == key::ENTER) {
    state = State::Exit;
  }
  else {
    state = State::Continue;
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
