#include "interfaces.hpp"

namespace termui {

Menu::Menu(const termui::string &t, const termui::strings &e, int ls)
    : title(t), elements(e), line_seperation(ls), cons(), list(elements, 1, 1), lvo(5), lho(4) {}
Menu::Menu(const std::string &t, const termui::strings &e, int ls) : title(t), elements(e), line_seperation(ls), cons(), list(elements, 1, 1), lvo(5), lho(4) {}
Menu::Menu(const std::string &t, const std::vector<std::string> &e, int ls)
    : title(t), elements(e), line_seperation(ls), cons(), list(elements, 1, 1), lvo(5), lho(4) {}

int Menu::show() {
  cons.show(); // configure terminal

  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();
  } while (state == state::CONTINUE);

  cons.close(); // reset terminal

  if (state == state::EXIT) {
    return -1;
  } else /* state == SELECT */ {
    return list.cursor; // returns selected option
  }
}

int Menu::cursor() { return list.cursor; }

termui::state Menu::status() { return state; }

void Menu::display() {
  update_size();

  cons.print(2, 2, bold_text(title.text()));
  cons.print(4, 2, list.render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select"));
  cons.flush();
}

void Menu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    list.cursor_up();
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    list.cursor_down();
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::ENTER) { // enter selects the option
    state = state::SELECT;

  } else if (ec == key::ESC) { // esc closes the interface
    state = state::EXIT;

  } else {
    state = state::CONTINUE;
  }
};

void Menu::update_size() {
  cons.update_size();

  list.h = cons.height - lvo;
  list.w = cons.width - lho;
  list.line_spacing = line_seperation;
}

} // namespace termui
