#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap Menu::styles({
  {"header", Style(Colors::White, 57, {SGR::Bold})}, 
  {"focus", Style(57, Color::Inherit())}, 
  {"blur", Styles::none}
});
// clang-format on

Text Menu::control_banner("[ESC] close  [↑/↓] scroll [↵] select", Styles::faint);

Menu::Menu(const termui::string &t, const termui::strings &e, unsigned int ls)
  : Interface(TermSetups::fullscreen),
    title(t),
    elements(e),
    title_banner(title, styles.at("header")),
    list(elements, styles.at("focus"), styles.at("blur")),
    line_seperation(ls) {}

int Menu::show() {
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

  if (state == Interface::State::Select) {
    return list.get_cursor();
  }
  else /* EXIT */ {
    return -1;
  }
}

unsigned int Menu::cursor() { return list.get_cursor(); }

termui::Interface::State Menu::status() { return state; }

void Menu::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.MoveCursor(2, 2);
  title_banner.render();

  terminal.MoveCursor(4, 2);
  list.render();

  terminal.MoveCursor(terminal.height(), 2);
  control_banner.render();

  terminal.flush();
}

void Menu::process_input() {
  std::string ec = terminal.read();

  if (ec == key::U_ARROW) {
    list.cursor_up();
    reprint = true;
  }
  else if (ec == key::D_ARROW) {
    list.cursor_down();
    reprint = true;
  }
  else if (ec == key::SHIFT_U_ARROW) {
    list.cursor_up(5);
    reprint = true;
  }
  else if (ec == key::SHIFT_D_ARROW) {
    list.cursor_down(5);
    reprint = true;
  }
  else if (ec == key::ENTER) { // enter selects the highlighted element
    state = State::Select;
  }
  else if (ec == key::ESC) { // esc closes the interface
    state = State::Exit;
  }
  else if (MouseInteraction(ec).match(EventType::ScrollUp)) {
    list.cursor_up();
    reprint = true;
  }
  else if (MouseInteraction(ec).match(EventType::ScrollDown)) {
    list.cursor_down();
    reprint = true;
  }
};

void Menu::update_size() {
  title_banner.width(std::min(terminal.width() - 2, (unsigned int)visible_length(title) + title_banner.hPadding())).height(1);
  list.width(terminal.width() - lho).height(terminal.height() - lvo).line_seperation(line_seperation);
  control_banner.width(terminal.width() - 2).height(1);
}

} // namespace termui
