#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap ToggleMenu::styles({
  {"header", Style(Colors::White, 57, {SGR::Bold})}, 
  {"focus", Style(212, std::nullopt, {SGR::Bold})}, 
  {"blur", Styles::none}
});
// clang-format on

Text ToggleMenu::control_banner("[ESC] close  [↑/↓] scroll [↵] toggle", Styles::faint);

ToggleMenu::ToggleMenu(const termui::string &t, const termui::strings &e, uint ls)
  : Interface(TermSetups::fullscreen),
    title(t),
    elements(e),
    title_banner(title, styles.at("header"), PaddingStyle::label),
    list(elements, styles.at("focus"), styles.at("blur")),
    line_seperation(ls) {}

bool ToggleMenu::isSelected(int i) { return list.getSelection(i); }

void ToggleMenu::show() {
  term_setup.configure();

  state = State::CONTINUE;
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();

  } while (state != State::EXIT); // cant break on select, thats the point of the interface

  term_setup.reset();
}

uint ToggleMenu::cursor() { return list.get_cursor(); }

void ToggleMenu::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.write(2, 2, title_banner.render());
  terminal.write(4, 2, list.render());
  terminal.write(terminal.height(), 2, control_banner.render());
  terminal.flush();
}

void ToggleMenu::process_input() {
  std::string ec = terminal.read();

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    list.cursor_up();
    reprint = true;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    list.cursor_down();
    reprint = true;

  } else if (ec == key::ENTER) { // enter toggles the option
    list.toggle();
    reprint = true;
    state = State::SELECT;

  } else if (ec == key::ESC) { // esc closes the interface
    state = State::EXIT;

  } else if (MouseInteraction(ec).match(EventType::ScrollUp)) {
    list.cursor_up();

  } else if (MouseInteraction(ec).match(EventType::ScrollDown)) {
    list.cursor_down();
  }
};

void ToggleMenu::update_size() {
  title_banner.width(std::min(terminal.width() - 2, (uint)visible_length(title) + title_banner.hPadding())).height(1);
  list.width(terminal.width() - lho).height(terminal.height() - lvo).line_seperation(line_seperation);
  control_banner.width(terminal.width() - lho).height(1);
}

} // namespace termui
