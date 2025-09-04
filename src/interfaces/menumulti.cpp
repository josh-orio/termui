#include "interfaces.hpp"

namespace termui {

MultiMenu::MultiMenu(const termui::string &t, const termui::strings &e, int ls)
    : title(t), elements(e), list(elements, 1, 1), line_seperation(ls), cons(), lvo(5), lho(4) {}
MultiMenu::MultiMenu(const std::string &t, const termui::strings &e, int ls)
    : title(t), elements(e), list(elements, 1, 1), line_seperation(ls), cons(), lvo(5), lho(4) {}
MultiMenu::MultiMenu(const std::string &t, const std::vector<std::string> &e, int ls)
    : title(t), list(elements, 1, 1), elements(e), line_seperation(ls), cons(), lvo(5), lho(4) {}

bool MultiMenu::isSelected(int i) { return list.getSelection(i); }

int MultiMenu::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close();       // reset terminal
  return list.cursor; // returns selected option
}

int MultiMenu::cursor() { return list.cursor; }

void MultiMenu::display() {
  update_size();

  cons.print(2, 2, bt(title.text()));
  cons.print(4, 2, list.render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select"));
  cons.flush();
}
bool MultiMenu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    list.cursor_up();
    return true;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    list.cursor_down();
    return true;

  } else if (ec == key::ENTER) { // enter toggles the option
    list.select();
    return true;

  } else if (ec == key::ESC) { // esc closes the interface
    return false;

  } else {
    return true;
  }

  return true;
};

void MultiMenu::update_size() {
  cons.update_size();

  list.h = cons.height - lvo;
  list.w = cons.width - lho;
  list.line_spacing = line_seperation;
}

} // namespace termui
