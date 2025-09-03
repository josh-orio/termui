#include "interfaces.hpp"

namespace termui {

FancyMenu::FancyMenu(const termui::string &title, const termui::strings &texts, const termui::strings &descs, int ls)
    : title(title), text(texts), desc(descs), list(text, desc, 1, 1), line_seperation(ls), cons() {}
FancyMenu::FancyMenu(const std::string &title, const std::vector<std::string> &texts, const std::vector<std::string> &descs, int ls)
    : title(title), text(texts), desc(descs), list(text, desc, 1, 1), line_seperation(ls), cons() {}

int FancyMenu::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close();  // reset terminal
  return cursor; // returns selected option
}

void FancyMenu::display() {
  update_size();

  cons.print(2, 2, bt(title.text()));
  cons.print(4, 2, list.render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select"));
  cons.flush();
}
bool FancyMenu::process_input() {
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

void FancyMenu::update_size() {
  cons.update_size();

  list.h = cons.height - voh;
  list.w = cons.width - hoh;
  list.line_spacing = line_seperation;
}

} // namespace termui
