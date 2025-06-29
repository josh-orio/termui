#include "interfaces.hpp"

namespace termui {

Table::Table(std::string t, std::vector<std::string> c, std::vector<nlohmann::json> d, int ch, int ls) {
  cons = Console(false, false, false, true);
  title = t;
  columns = c;
  data = d;
  cell_height = ch;
  line_seperation = ls;

  cursor = 0;
  start_line = 0;
}

int Table::show() {
  cons.show(); // configure terminal

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
      header += " " + std::string(c.begin(), c.begin() + column_width - 5) + "... ";
    } else if (c.size() <= column_width - 2) {
      header += " " + c + std::string((column_width - 2) - c.size(), ' ') + " ";
    }
    header += "│";
  }

  cons.print_ln(" " + title);
  cons.print_ln();
  cons.print_ln(" " + headline);
  cons.print_ln(" " + bold_text(header));
  cons.print_ln(" " + splitter);

  std::vector<std::string> row_text;
  std::string cell_text;
  int char_lim;
  int space_used = 0;
  for (int i = start_line; i < std::min((int)data.size(), start_line + visible_rows); i++) {

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
        cell_text = std::string(cell_text.begin(), cell_text.begin() + char_lim - 3) + "...";
      }

      for (int ii = 0; ii < cell_height; ii++) {
        row_text[ii] += " " +
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
      if (space_used < cons.height - overhead) {
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

  overhead = 7; // header(2) + table header(3) + table foot(1) + footer(1)

  visible_rows = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (cell_height * (visible_rows + 1)) + (line_seperation * visible_rows);

    if (space_used > (cons.height - overhead)) {
      break;
    }
    visible_rows++;
  }

  table_width = cons.width - 2; // take space on each side of table
  column_width = (table_width - (columns.size() + 1)) / columns.size();
}

} // namespace termui
