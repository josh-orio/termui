#include "elements.hpp"

namespace termui {

Table::Table(std::vector<std::string> columns, std::vector<int> column_widths,
             std::vector<std::vector<std::string>> cells)
    : columns(columns), column_widths(column_widths), cells(cells) {

  table_width = 1;
  for (auto c : column_widths) {
    table_width += c + 1;
  }

  table_height = 20; // need to reimplement cell height and line sep

  start_line = 0;
  cursor = 1;
  overhead = 3 + 1; // header(3) + footer(1)
  visible_rows = table_height - overhead;

  cell_height = 1;

  row_color = bg_color(56);
}

std::string Table::render() {
  std::string outbuff;
  outbuff += Box(table_width, table_height, fg_color(238)).render();

  outbuff += curs_up(table_height - 2) + curs_left(table_width - 1);

  std::string header;
  for (int i = 0; i < columns.size(); i++) {
    if (columns[i].size() > column_widths[i] - 2) {
      header += Text(columns[i], column_widths[i], 1, "", "").render();
    } else if (columns[i].size() <= column_widths[i] - 2) {
      header += Text(columns[i], column_widths[i], 1, "", "").render();
    }
    if (i != columns.size() - 1) {
      header += " ";
    }
  }

  outbuff += header;

  outbuff += curs_down(1) + curs_left(table_width - 2);
  outbuff += fg_color(238) + div_line(table_width - 2) + format::NONE;
  outbuff += curs_down(1) + curs_left(table_width - 2);

  std::string row_text; // just a buffer for this row output
  std::string cell_text;
  for (int i = start_line; i < std::min((int)cells.size(), start_line + visible_rows); i++) {
    row_text = "";

    if (i == cursor) {
      row_text += Text("", table_width - 2, cell_height, "", row_color)
                      .render(); // this is plain inefficient (filling in bg for highlighted row)
      if (cell_height > 1) {
        row_text += curs_up(cell_height - 1); // ansi 0 arg treated as 1
      }
      row_text += curs_left(table_width - 2); // go back to row origin, up if cell multiline, left to start of col 0
    }

    for (int ii = 0; ii < columns.size(); ii++) {
      if (i == cursor) {
        cell_text = Text(cells[i][ii], column_widths[i], cell_height, "", row_color).render();
      } else {
        cell_text = Text(cells[i][ii], column_widths[i], cell_height, "", "").render();
      }

      row_text += cell_text;
      if (ii != columns.size() - 1) { // if not last column
        if (cell_height > 1) {
          row_text += curs_up(cell_height - 1); // ansi 0 arg treated as 1
        }
        row_text += curs_right(1); // go up if cell is multiline, right to next col
      }
    }

    outbuff += row_text;
    outbuff += curs_down(1) + curs_left(table_width - 2); // go down (line sep), got to col 0 of table
  }

  return outbuff;
}

} // namespace termui
