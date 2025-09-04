#include "interfaces.hpp"

namespace termui {

Menu::Menu(const termui::string &t, const termui::strings &e, int ls) : title(t), elements(e), list(elements, 1, 1), line_seperation(ls), cons() {}
Menu::Menu(const std::string &t, const termui::strings &e, int ls) : title(t), elements(e), list(elements, 1, 1), line_seperation(ls), cons() {}
Menu::Menu(const std::string &t, const std::vector<std::string> &e, int ls) : title(t), elements(e), list(elements, 1, 1), line_seperation(ls), cons() {}

int Menu::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close();       // reset terminal
  return list.cursor; // returns selected option
}

void Menu::display() {
  update_size();

  cons.print(2, 2, bt(title.text()));
  cons.print(4, 2, list.render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select"));
  cons.flush();
}
bool Menu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    list.cursor_up();
    return true;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    list.cursor_down();
    return true;

  } else if (ec == key::ENTER) { // enter selects the option
    return false;

  } else if (ec == key::ESC) { // esc closes the interface
    return false;

  } else {
    return true;
  }
};

void Menu::update_size() {
  cons.update_size();

  list.h = cons.height - voh;
  list.w = cons.width - hoh;
  list.line_spacing = line_seperation;
}

} // namespace termui
