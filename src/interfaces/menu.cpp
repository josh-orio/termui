#include "interfaces.hpp"

namespace termui {

Menu::Menu(const std::string &t, const std::vector<std::string> &o, int ls) : title(t), options(o) {
  cons = Console(false, false, false, true);
  line_seperation = ls;
  cursor = 0;
  start_line = 0;
  overhead = 4; // header(3) + footer(1)
}

int Menu::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close(); // reset terminal

  return cursor; // returns selected option
}

void Menu::display() {
  update_size();

  cons.print(2, 2, title);

  int space_used = 0;
  for (int i = start_line; i < std::min((int)options.size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      cons.print(space_used + 4, 2, bt("> " + options[i]));
    } else {
      cons.print(space_used + 4, 4, options[i]);
    }
    space_used++;

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - overhead) {
        space_used++;
      }
    }
  }

  cons.print(cons.height, 2, faint_text("[↵] select  [↑/↓] scroll"));
  cons.flush();
}
bool Menu::process_input() {
  std::vector<std::string> controls{key::U_ARROW, key::D_ARROW, key::ENTER}; // replacing this will be part of #12
  std::string ec = cons.poll_input();                                        // read in a control

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    cursor -= (cursor > 0) ? 1 : 0;
    start_line -= (cursor < start_line) ? 1 : 0;
    return true;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    cursor += (cursor < options.size() - 1) ? 1 : 0;
    start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
    return true;

  } else if (ec == key::ENTER) { // enter selects the option
    return false;

  } else {
    return true;
  }
};

void Menu::update_size() {
  cons.update_size();

  visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_lines + 1) + (line_seperation * visible_lines);

    if (space_used > (cons.height - overhead)) {
      break;
    }
    visible_lines++;
  }
}

} // namespace termui
