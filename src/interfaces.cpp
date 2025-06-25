#include "interfaces.hpp"
#include "formatting.hpp"
#include <string>

Info::Info(std::string t, std::string c) {
  cons = Console(false, false, false, true);
  title = t;
  content = c;
}

void Info::show() {
  cons.show(); // configure terminal

  line_cursor = 0;

  do {
    display();
  } while (await_input());

  cons.close(); // reset terminal
}

void Info::display() {
  update_size();

  std::vector<std::string> formatted = {};
  std::string copy = content;

  // a bit of preprocessing to fit the text in the terminal
  while (copy.length() > 0) {
    int next = 0;
    if (copy.find('\n') == std::string::npos) {
      next = std::min({(int)copy.size(), text_width});
    } else {
      next = std::min({(int)copy.size(), text_width, (int)copy.find('\n')});
    }

    formatted.push_back(std::string(copy.begin(), copy.begin() + next));

    if (copy.begin() + next + 1 < copy.end()) {
      if (copy[next] == '\n') {
        copy = std::string(copy.begin() + next + 1, copy.end());
      } else {
        copy = std::string(copy.begin() + next, copy.end());
      }
    } else {
      copy = "";
    }
  }

  content_lines = formatted.size();

  cons.clear();

  cons.print_ln(" " + title);
  cons.print_ln(" " + faint_text(div_line(cons.width - 2)));

  for (int i = line_cursor;
       i < std::min(line_cursor + visible_lines, (int)formatted.size()); i++) {
    cons.print_ln("  " + formatted[i]);
  }

  cons.print_at_pos(faint_text("[←] return  [↑/↓] scroll"), cons.height, 2);
}

bool Info::await_input() {
  if (std::getchar() == keys::ESC) {
    if (std::getchar() == '[') { // random char in escape sequence
      switch (std::getchar()) {
      case keys::U_ARROW:
        // decrement but dont let (cursor < 0)
        line_cursor -= (line_cursor > 0) ? 1 : 0;
        return true;

      case keys::D_ARROW:
        // increment but dont let (cursor > options.size)
        line_cursor += (line_cursor < content_lines - 1) ? 1 : 0;
        return true;

      case keys::L_ARROW:
        // left arrow closes info page
        return false;
      }
    }
  }

  return true;
}

void Info::update_size() {
  cons.update_size();

  text_width = cons.width - 4; // 2 space padding on each side
  visible_lines = cons.height -
                  3; // take three lines off for header (2) and footer (1) space
}

Input::Input(std::string t, std::vector<std::string> f) {
  cons = Console(false, false, false, true);
  title = t;
  fields = f;
  responses.resize(fields.size(), "");
  line_seperation = 0;
}

Input::Input(std::string t, std::vector<std::string> f, int ls) {
  cons = Console(false, false, false, true);
  title = t;
  fields = f;
  responses.resize(fields.size(), "");
  line_seperation = ls;
}

int Input::show() {
  cons.show(); // configure terminal

  cursor = 0;
  start_line = 0;
  selected = false;

  int sig;
  do {
    display();
    sig = await_input();
  } while (sig != -1);

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

std::vector<std::string> Input::get_responses() { return responses; }

void Input::display() {
  update_size();
  cons.clear();

  cons.print_ln(" " + title);
  cons.print_ln(" " + faint_text(div_line(cons.width - 2)));

  int space_used = 0;
  for (int i = start_line;
       i < std::min((int)fields.size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      if (selected) {
        cons.print_ln(bt(" > " + fields[i]) + ": " + rv(responses[i] + " "));
      } else {
        cons.print_ln(bt(" > " + fields[i]) + ": " + responses[i]);
      }
    } else {
      cons.print_ln("   " + fields[i] + ": " + responses[i]);
    }
    space_used++;

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - 3) {
        cons.print_ln();
        space_used++;
      }
    }
  }
  cons.print_at_pos(
      faint_text("[↵] select/deselect  [↑/↓] scroll [→] finalize"), cons.height,
      2);
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
      // enter signals (de)selection of current field
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
        responses[cursor] =
            std::string(responses[cursor].begin(), responses[cursor].end() - 1);
      }
      return 0;
    }
  } else {
    switch (c = std::getchar()) {
    case keys::ENTER:
      // enter signals (de)selection of current field
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

    if (space_used > (cons.height - 3)) { // take 3 for header and footer
      break;
    }
    visible_lines++;
  }
}

Menu::Menu(std::string t, std::vector<std::string> o) {
  cons = Console(false, false, false, true);
  title = t;
  options = o;
  line_seperation = 0;
}

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
  for (int i = start_line;
       i < std::min((int)options.size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      cons.print_ln(bt(" > " + options[i]));
    } else {
      cons.print_ln(ws(3) + options[i]);
    }
    space_used++;

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - 3) {
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

  visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_lines + 1) + (line_seperation * visible_lines);

    if (space_used > (cons.height - 3)) { // take 3 for header and footer
      break;
    }
    visible_lines++;
  }
}

Table::Table(std::string t, std::vector<std::string> c,
             std::vector<nlohmann::json> d) {
  cons = Console(false, false, false, true);
  title = t;
  columns = c;
  data = d;
  cell_height = 1;
  line_seperation = 0;
}

Table::Table(std::string t, std::vector<std::string> c,
             std::vector<nlohmann::json> d, int ch, int ls) {
  cons = Console(false, false, false, true);
  title = t;
  columns = c;
  data = d;
  cell_height = ch;
  line_seperation = ls;
}

int Table::show() {
  cons.show(); // configure terminal

  cursor = 0;
  start_line = 0;

  int sig;
  do {
    display();
    sig = await_input();
  } while (sig != -1);

  cons.close(); // reset terminal

  // Return codes:
  // -1 : normal exit
  // >= 0 : selection at cursor position
  if (sig == -1) {
    return -1;
  } else {
    return cursor;
  }
}

void Table::display() {
  update_size();
  cons.clear();

  // construct strings for table headers, and columns
  // some of this could get replaced by std::format (?)
  std::string headline = "╭";
  std::string splitter = "├";
  std::string spacing = "│";
  std::string footer = "╰";

  for (int i = 0; i < columns.size(); i++) {
    headline += div_line(column_width);
    splitter += div_line(column_width);
    spacing += ws(column_width);
    footer += div_line(column_width);

    if (i < columns.size() - 1) {
      headline += "┬";
      splitter += "┼";
      spacing += "│";
      footer += "┴";
    }
  }

  headline += "╮";
  splitter += "┤";
  spacing += "│";
  footer += "╯";

  std::string header = "│";
  for (std::string c : columns) {
    if (c.size() > column_width - 2) {
      header +=
          " " + std::string(c.begin(), c.begin() + column_width - 5) + "... ";
    } else if (c.size() <= column_width - 2) {
      header += " " + c + std::string((column_width - 2) - c.size(), ' ') + " ";
    }
    header += "│";
  }

  cons.print_ln(" " + title);
  cons.print_ln(" " + faint_text(div_line(cons.width - 2)));
  cons.print_ln(" " + headline);
  cons.print_ln(" " + bold_text(header));
  cons.print_ln(" " + splitter);

  std::vector<std::string> row_text;
  std::string cell_text;
  int char_lim;
  int space_used = 0;
  for (int i = start_line;
       i < std::min((int)data.size(), start_line + visible_rows); i++) {

    row_text = std::vector<std::string>(cell_height, "│");

    for (std::string c : columns) {
      char_lim = cell_height * (column_width - 2);

      if (data[i].contains(c)) {
        cell_text = data[i][c].get<std::string>();
      } else {
        cell_text = "ERROR";
      }

      if (cell_text.length() < char_lim) {
        cell_text += std::string(char_lim - cell_text.length(), ' ');
      } else if (cell_text.length() > char_lim) {
        cell_text =
            std::string(cell_text.begin(), cell_text.begin() + char_lim - 3) +
            "...";
      }

      for (int ii = 0; ii < cell_height; ii++) {
        row_text[ii] +=
            " " +
            std::string(cell_text.begin() + (ii * (column_width - 2)),
                        cell_text.begin() + ((ii + 1) * (column_width - 2))) +
            " │";
      }
    }

    for (int ii = 0; ii < cell_height; ii++) {
      if (i == cursor) {
        cons.print_ln(" " + rv(row_text[ii]));
      } else {
        cons.print_ln(" " + row_text[ii]);
      }

      space_used++;
    }

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - 7) {
        cons.print_ln(" " + spacing);
        space_used++;
      }
    }
  }
  cons.print_ln(" " + footer);
  cons.print_at_pos(faint_text("[←] exit  [↑/↓] scroll"), cons.height, 2);
}

int Table::await_input() {
  // return values:
  // -1: close window
  // 0: normal/proceed
  // 1: element selected

  switch (std::getchar()) {
  case keys::ENTER:
    return 1;

  case keys::ESC:
    if (std::getchar() == '[') { // random char in escape sequence
      switch (std::getchar()) {
      case keys::U_ARROW:
        // decrement but dont let (cursor < 0)
        cursor -= (cursor > 0) ? 1 : 0;
        start_line -= (cursor < start_line) ? 1 : 0;
        return 0;

      case keys::D_ARROW:
        // increment but dont let (cursor > options.size)
        cursor += (cursor < data.size() - 1) ? 1 : 0;
        start_line += (cursor >= start_line + visible_rows) ? 1 : 0;
        return 0;

      case keys::L_ARROW:
        // left arrow closes info page
        return -1;
      }
    }
  }

  return 0;
}

void Table::update_size() {
  cons.update_size();

  visible_rows = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used =
        (cell_height * (visible_rows + 1)) + (line_seperation * visible_rows);

    if (space_used >
        (cons.height - 7)) { // take 7 for header, table header and footer
      break;
    }
    visible_rows++;
  }

  table_width = cons.width - 2; // take space on each side of table
  column_width = (table_width - (columns.size() + 1)) / columns.size();
}
