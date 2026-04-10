#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap InfoPage::styles ({
    {"header", Style(Colors::White, 57, {SGR::Bold})}
});
// clang-format on

Text InfoPage::control_strip("[ESC] close  [↑↓] navigate  [shift + ↑↓] fast navigate", Styles::faint);

InfoPage::InfoPage(const termui::string &t, const termui::string &c)
  : Interface(TermSetups::fullscreen), title(t), content(c), title_banner(title, styles.at("header")), info(content, Styles::none) {}

void InfoPage::show() {
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

void InfoPage::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.write(2, 2, title_banner.render());
  terminal.write(4, 3, info.render());
  terminal.write(terminal.height(), 2, control_strip.render());

  terminal.flush();
}

void InfoPage::process_input() {
  std::string ec = terminal.read();

  if (ec == key::U_ARROW) {
    info.cursor_up();
    reprint = true;

  } else if (ec == key::SHIFT_U_ARROW) {
    info.cursor_up(5);
    reprint = true;

  } else if (ec == key::D_ARROW) {
    info.cursor_down();
    reprint = true;

  } else if (ec == key::SHIFT_D_ARROW) {
    info.cursor_down(5);
    reprint = true;

  } else if (MouseInteraction(ec).match(EventType::ScrollUp)) {
    info.cursor_up();
    reprint = true;

  } else if (MouseInteraction(ec).match(EventType::ScrollDown)) {
    info.cursor_down();
    reprint = true;

  } else if (ec == key::ESC) {
    state = State::EXIT;
  }
}

void InfoPage::update_size() {
  title_banner.width(std::min(terminal.width() - 2, (uint)visible_length(title) + title_banner.hPadding())).height(1);
  info.width(terminal.width() - hoh).height(terminal.height() - voh);
  control_strip.width(terminal.width() - 2).height(1);
}

} // namespace termui
