#include "interfaces.hpp"

namespace termui {

Menu::Menu(std::string t, std::vector<std::string> o, int s) {
  cons = Console(false, false, false, true);
  title = t;
  options = o;
  line_seperation = s;
}

int Menu::show() {
  cons.show(); // configure terminal

  cursor = 0;
  start_line = 0;

  do {
    display();
  } while (await_input());

  cons.close(); // reset terminal

  return cursor; // returns selected option
}

void Menu::display() {
  update_size();
  cons.clear();

  cons.print_ln(" " + title);
  cons.print_ln(" " + faint_text(div_line(cons.width - 2)));

  int space_used = 0;
  for (int i = start_line; i < std::min((int)options.size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      cons.print_ln(bt(" > " + options[i]));
    } else {
      cons.print_ln(ws(3) + options[i]);
    }
    space_used++;

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - overhead) {
        cons.print_ln();
        space_used++;
      }
    }
  }

  cons.print_at_pos(faint_text("[↵] select  [↑/↓] scroll"), cons.height, 2);
}

bool Menu::await_input() {
  switch (std::getchar()) {
  case keys::ENTER:
    return false;

  case keys::ESC:
    if (std::getchar() == '[') { // random char in escape sequence
      switch (std::getchar()) {
      case keys::U_ARROW:
        // decrement but dont let (cursor <0)
        cursor -= (cursor > 0) ? 1 : 0;
        start_line -= (cursor < start_line) ? 1 : 0;
        return true;

      case keys::D_ARROW:
        // increment but dont let (cursor > options.size)
        cursor += (cursor < options.size() - 1) ? 1 : 0;
        start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
        return true;
      }
    }
  }

  return true;
}

void Menu::update_size() {
  cons.update_size();

  overhead = 3; // header(2) + footer(1)

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
