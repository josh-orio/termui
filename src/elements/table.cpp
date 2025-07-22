#include "elements.hpp"

namespace termui {

Table::Table(std::vector<std::string> columns, std::vector<std::vector<std::string>> cells, int table_height,
             std::vector<int> column_widths, int cell_height, int line_seperation)
    : columns(columns), cells(cells), table_height(table_height), column_widths(column_widths),
      cell_height(cell_height), line_seperation(line_seperation) {

  table_width = 1;
  for (auto c : column_widths) {
    table_width += c + 1;
  }

  // table_height = 20; // need to reimplement cell height and line sep

  start_line = 0;
  cursor = 0;
  overhead = 3 + 1; // header(3) + footer(1)

  cell_height = 1;

  active_color = bg_color(56);
  box_color = fg_color(238);
}

std::string Table::render() {
  internal_update();

  std::string outbuff;
  outbuff += Box(table_width, table_height, box_color).render();

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
  outbuff += fg_apply(div_line(table_width - 2), box_color);
  outbuff += curs_down(1) + curs_left(table_width - 2);

  std::string row_text; // just a buffer for this row output
  std::string cell_text;
  for (int i = start_line; i < std::min((int)cells.size(), start_line + visible_rows); i++) {
    row_text = "";

    if (i == cursor) {
      row_text += Text("", table_width - 2, cell_height, "", active_color)
                      .render(); // this is plain inefficient (filling in bg for highlighted row)
      if (cell_height > 1) {
        row_text += curs_up(cell_height - 1); // ansi 0 arg treated as 1
      }
      row_text += curs_left(table_width - 2); // go back to row origin, up if cell multiline, left to start of col 0
    }

    for (int ii = 0; ii < columns.size(); ii++) {
      if (i == cursor) {
        cell_text = Text(cells[i][ii], column_widths[i], cell_height, "", active_color).render();
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
    outbuff += curs_down(line_seperation + 1) + curs_left(table_width - 2); // go down (line sep), got to col 0 of table
  }

  return outbuff;
}

void Table::cursor_up() { // decrement but dont let (cursor < 0)
  internal_update();
  cursor -= (cursor > 0) ? 1 : 0;
  start_line -= (cursor < start_line) ? 1 : 0;
}

void Table::cursor_down() { // increment but dont let (cursor > options.size)
  internal_update();
  cursor += (cursor < cells.size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + visible_rows) ? 1 : 0;
}

void Table::internal_update() {
  visible_rows = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_rows + 1) + (line_seperation * visible_rows);

    if (space_used > table_height - overhead) {
      break;
    }
    visible_rows++;
  }

  if (cursor < start_line || start_line + visible_rows < cursor) {
    start_line = cursor;
  }
}

} // namespace termui
