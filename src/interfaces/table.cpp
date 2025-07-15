// #include "interfaces.hpp"
// #include "util.hpp"

// namespace termui {

// Table::Table(const std::string &t, const std::vector<std::string> &c, const std::vector<nlohmann::json> &d, int ch,
//              int ls)
//     : title(t), columns(c), data(d) {
//   cons = Console(false, false, false, true);
//   cell_height = ch;
//   line_seperation = ls;
//   cursor = 0;
//   start_line = 0;
//   overhead = 8; // header(3) + table header(3) + table foot(1) + footer(1)
// }

// int Table::show() {
//   cons.show(); // configure terminal

//   reprint = true;

//   int sig;
//   do {
//     display();
//     sig = process_input();
//   } while (sig == 0);

//   cons.close(); // reset terminal

//   // Return codes:
//   // -1 : normal exit
//   // >= 0 : selection at cursor position
//   if (sig == -1) {
//     return -1;
//   } else {
//     return cursor;
//   }
// }

// void Table::display() {
//   update_size();
//   // cons.clear();

//   // construct strings for table headers, and columns
//   // some of this could get replaced by std::format (?)
//   std::string headline = "╭";
//   std::string splitter = "├";
//   std::string spacing = "│";
//   std::string footer = "╰";

//   for (int i = 0; i < columns.size(); i++) {
//     headline += div_line(column_width);
//     splitter += div_line(column_width);
//     spacing += ws(column_width);
//     footer += div_line(column_width);

//     if (i < columns.size() - 1) {
//       headline += "┬";
//       splitter += "┼";
//       spacing += "│";
//       footer += "┴";
//     }
//   }

//   headline += "╮";
//   splitter += "┤";
//   spacing += "│";
//   footer += "╯";

//   std::string header = "│";
//   for (std::string c : columns) {
//     if (c.size() > column_width - 2) {
//       header += " " + std::string(c.begin(), c.begin() + column_width - 5) + "... ";
//     } else if (c.size() <= column_width - 2) {
//       header += " " + c + std::string((column_width - 2) - c.size(), ' ') + " ";
//     }
//     header += "│";
//   }

//   cons.print(2, 2, title);
//   cons.print(4, 2, headline);
//   cons.print(5, 2, bold_text(header));
//   cons.print(6, 2, splitter);

//   std::vector<std::string> row_text;
//   std::string cell_text;
//   int char_lim;
//   int space_used = 0;
//   for (int i = start_line; i < std::min((int)data.size(), start_line + visible_rows); i++) {

//     row_text = std::vector<std::string>(cell_height, "│");

//     for (std::string c : columns) {
//       char_lim = cell_height * (column_width - 2);

//       if (data[i].contains(c)) {
//         cell_text = data[i][c].get<std::string>();
//       } else {
//         cell_text = "ERROR";
//       }

//       if (cell_text.length() < char_lim) {
//         cell_text += std::string(char_lim - cell_text.length(), ' ');
//       } else if (cell_text.length() > char_lim) {
//         cell_text = std::string(cell_text.begin(), cell_text.begin() + char_lim - 3) + "...";
//       }

//       for (int ii = 0; ii < cell_height; ii++) {
//         row_text[ii] += " " +
//                         std::string(cell_text.begin() + (ii * (column_width - 2)),
//                                     cell_text.begin() + ((ii + 1) * (column_width - 2))) +
//                         " │";
//       }
//     }

//     for (int ii = 0; ii < cell_height; ii++) {
//       if (i == cursor) {
//         cons.print(space_used + 7, 2, rv(row_text[ii]));
//       } else {
//         cons.print(space_used + 7, 2, row_text[ii]);
//       }

//       space_used++;
//     }

//     for (int ii = 0; ii < line_seperation; ii++) {
//       if (space_used < cons.height - overhead) {
//         cons.print(space_used + 7, 2, spacing);
//         space_used++;
//       }
//     }
//   }

//   cons.print(space_used + 7, 2, footer);
//   cons.print(cons.height, 2, faint_text("[ESC] exit  [↑/↓] scroll [↵] select"));
//   cons.flush();
// }
// int Table::process_input() {
//   // return values:
//   // -1: close window
//   // 0: normal/proceed
//   // 1: element selected

//   std::vector<std::string> controls{key::ENTER, key::U_ARROW, key::D_ARROW,
//                                     key::ESC}; // replacing this will be part of #12
//   std::string ec = cons.poll_input();          // read in a control

//   if (ec == key::ENTER) {
//     return 1;
//   } else if (ec == key::U_ARROW) {
//     // decrement but dont let (cursor < 0)
//     cursor -= (cursor > 0) ? 1 : 0;
//     start_line -= (cursor < start_line) ? 1 : 0;

//   } else if (ec == key::D_ARROW) {
//     // increment but dont let (cursor > options.size)
//     cursor += (cursor < data.size() - 1) ? 1 : 0;
//     start_line += (cursor >= start_line + visible_rows) ? 1 : 0;

//   } else if (ec == key::ESC) {
//     return -1;

//   } else {
//     return 0;
//   }
// };

// void Table::update_size() {
//   cons.update_size();

//   visible_rows = 0;
//   while (true) {
//     // calculates lines used by displaying another row of cells
//     int space_used = (cell_height * (visible_rows + 1)) + (line_seperation * visible_rows);

//     if (space_used > (cons.height - overhead)) {
//       break;
//     }
//     visible_rows++;
//   }

//   table_width = cons.width - 2; // take space on each side of table
//   column_width = (table_width - (columns.size() + 1)) / columns.size();
// }

// } // namespace termui
