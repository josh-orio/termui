#include <termui/ui/ui.hpp>

namespace termui {

// clang-format off
StyleMap FancyMenu::styles({
  {"header", Style(Colors::White, 57, {SGR::Bold})}, 
  {"focus", Style(212, std::nullopt, {SGR::Bold})}, 
  {"blur", Styles::none}
});
// clang-format on

Text FancyMenu::control_banner("[ESC] close  [↑/↓] scroll [↵] select", Styles::faint);

FancyMenu::FancyMenu(const termui::string &title, const std::vector<FancyList::Element> &rows, uint line_seperation)
  : Interface(TermSetups::fullscreen),
    title(title),
    title_banner(title, styles.at("header"), PaddingStyle::label),
    list(rows, styles.at("focus"), styles.at("blur")),
    line_seperation(line_seperation) {}

termui::Interface::State FancyMenu::show() {
  term_setup.configure();

  state = Interface::State::CONTINUE;
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();

  } while (state == State::CONTINUE);

  term_setup.reset();

  return state;
}

uint FancyMenu::cursor() { return list.get_cursor(); }

void FancyMenu::display() {
  update_size();

  terminal.ClearScreen();
  terminal.ClearScrollback();

  terminal.write(2, 2, title_banner.render());
  terminal.write(4, 2, list.render());
  terminal.write(terminal.height(), 2, control_banner.render());
  terminal.flush();
}

void FancyMenu::process_input() {
  std::string ec = terminal.read();

  if (ec == key::U_ARROW) {
    list.cursor_up();
    reprint = true;

  } else if (ec == key::D_ARROW) {
    list.cursor_down();
    reprint = true;

  } else if (ec == key::ENTER) { // enter selects the option
    state = State::SELECT;

  } else if (ec == key::ESC) { // esc closes the interface
    state = State::EXIT;

  } else if (MouseInteraction(ec).match(EventType::ScrollUp)) {
    list.cursor_up();

  } else if (MouseInteraction(ec).match(EventType::ScrollDown)) {
    list.cursor_down();
  }
};

void FancyMenu::update_size() {
  title_banner.width(std::min(terminal.width() - 2, (uint)visible_length(title) + title_banner.hPadding())).height(1);
  list.width(terminal.width() - lho).height(terminal.height() - lvo).line_seperation(line_seperation);
  control_banner.width(terminal.width() - lho).height(1);
}

} // namespace termui
