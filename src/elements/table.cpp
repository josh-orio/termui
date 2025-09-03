#include "elements.hpp"

namespace termui {

Table::Table(const termui::strings &cols, const std::vector<termui::strings> &rows, std::vector<int> colwidths, int table_height, int cell_height,
             int line_seperation, int col)
    : cols(cols), colwidths(colwidths), rows(rows), table_height(table_height), cell_height(cell_height), line_seperation(line_seperation), active_color(col),
      cursor(0), start_line(0), box_color(clr::DARKGREY), overhead(4) {}

const termui::strings &Table::getCols() const { return cols; }
termui::strings &Table::getCols() { return cols; }

const int &Table::colWidth(int i) const { return colwidths.at(i); }
int &Table::colWidth(int i) { return colwidths.at(i); }

const termui::strings &Table::getRow(int i) const { return rows.at(i); }
termui::strings &Table::getRow(int i) { return rows.at(i); }

const std::string &Table::getCell(int row, int col) const { return rows.at(row).getItem(col); }
std::string &Table::getCell(int row, int col) { return rows.at(row).getItem(col); }

int Table::colCount() { return cols.size(); }
int Table::rowCount() { return rows.size(); }

std::string Table::render() {
  internal_update();

  std::string outbuff;

  outbuff += Box(table_width, table_height, box_color).render();
  outbuff += curs_up(table_height - 2) + curs_left(table_width - 1);

  std::string header;
  for (int i = 0; i < cols.size(); i++) {
    header += Text(cols.getItem(i), colwidths.at(i), 1, clr::DEFAULT, clr::DEFAULT).render();

    if (i != cols.size() - 1) {
      header += " ";
    }
  }

  outbuff += header;
  outbuff += curs_down(1) + curs_left(table_width - 2);
  outbuff += fg_apply(div_line(table_width - 2), box_color);
  outbuff += curs_down(1) + curs_left(table_width - 2);

  std::string row_text; // just a buffer for this row output
  std::string cell_text;

  for (int i = start_line; i < std::min((int)rows.size(), start_line + visible_rows); i++) {
    row_text = "";

    if (i == cursor) {
      row_text += Text("", table_width - 2, cell_height, clr::DEFAULT, active_color).render(); // this is plain inefficient (filling in bg for highlighted row)
      if (cell_height > 1) {
        row_text += curs_up(cell_height - 1); // ansi 0 arg treated as 1
      }
      row_text += curs_left(table_width - 2); // go back to row origin, up if cell multiline, left to start of col 0
    }

    for (int ii = 0; ii < cols.size(); ii++) {
      if (i == cursor) {
        cell_text = Text(rows.at(i).getItem(ii), /*(*rows).at(i).cells->at(ii).data(),*/ colwidths.at(i), cell_height, clr::DEFAULT, active_color).render();
      } else {
        cell_text = Text(rows.at(i).getItem(ii), /*(*rows).at(i).cells->at(ii).data(),*/ colwidths.at(i), cell_height, clr::DEFAULT, clr::DEFAULT).render();
      }

      row_text += cell_text;
      if (ii != cols.size() - 1) { // if not last column
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

void Table::cursor_down() { // increment but dont let (cursor > rows.size)
  internal_update();
  cursor += (cursor < rows.size() - 1) ? 1 : 0;
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

  table_width = 1;
  for (auto &c : colwidths) {
    table_width += c + 1;
  }
}

} // namespace termui
