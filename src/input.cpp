#include "interfaces.hpp"

namespace termui {

Input::Input(const std::string &t, const std::vector<std::string> &f, const std::vector<std::string> &r, int ls)
    : title(t), fields(f), responses(r) {
  cons = Console(false, false, false, true);
  line_seperation = ls;
  cursor = 0;
  start_line = 0;
  overhead = 4; // header(3) + footer(1)
}

int Input::show() {
  cons.show(); // configure terminal

  selected = false;

  int sig = 0;
  do {
    display();
    sig = await_input();
  } while (sig == 0);

  cons.close(); // reset terminal

  // return codes:
  // -1 : normal exit
  // >= 0 : selection at cursor position
  if (sig == -1) {
    return -1;
  } else {
    return cursor;
  }
}

// std::vector<std::string> Input::get_responses() { return responses; }

void Input::display() {
  update_size();

  cons.print(2, 2, title);

  int space_used = 0;
  for (int i = start_line; i < std::min((int)fields.size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      if (selected) {
        cons.print(space_used + 4, 2, bt("> " + fields[i]) + ": " + rv(responses[i] + " "));
      } else {
        cons.print(space_used + 4, 2, bt("> " + fields[i]) + ": " + responses[i]);
      }
    } else {
      cons.print(space_used + 4, 4, fields[i] + ": " + responses[i]);
    }
    space_used++;

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - overhead) {
        space_used++;
      }
    }
  }

  cons.print(cons.height, 2, faint_text("[↵] select/deselect  [↑/↓] scroll [→] finalize"));
  cons.flush();
}

int Input::await_input() {
  // return values:
  // -1: close window
  // 0: normal/proceed
  // 1: element selected

  char c;
  if (selected) {
    switch (c = std::getchar()) {
    case keys::ENTER:
      // enter signals deselection of current field
      selected = false;
      return 0;

    case keys::ESC:
      // ESC gets ignored plus whatever 2 chars complete the sequence
      std::getchar();
      std::getchar();
      return 0;

    case 32 ... 126:
      // accept ascii characters in range 32 - 126
      // input sanitization is done in classes, not here
      responses[cursor] += c;
      return 0;

    case keys::DEL:
      // DEL char clears last char in string
      if (responses[cursor].size() > 0) {
        responses[cursor] = std::string(responses[cursor].begin(), responses[cursor].end() - 1);
      }
      return 0;
    }
  } else {
    switch (c = std::getchar()) {
    case keys::ENTER:
      // enter signals selection of current field
      selected = true;
      return 1;

    case keys::ESC:
      if (std::getchar() == '[') {
        switch (c = std::getchar()) {
        case keys::U_ARROW:
          // up arrow moves cursor up
          // decrement but dont let (cursor < 0)
          cursor -= (cursor > 0) ? 1 : 0;
          start_line -= (cursor < start_line) ? 1 : 0;
          return 0;

        case keys::D_ARROW:
          // down arrow moves cursor down
          // increment but dont let (cursor > fields.size)
          cursor += (cursor < fields.size() - 1) ? 1 : 0;
          start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
          return 0;

        case keys::R_ARROW:
          // right arrow finalizes the responses
          // can't return if a field is selected (good thing)
          return -1;
        }
      }
    }
  }
  return 0; // i guess?
}

void Input::update_size() {
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
