#include <termui/ui/ui.hpp>

namespace termui {

Table::Column::Column(termui::string str, uint w) : title(str), width(w) {};

Table::Row::Row(termui::strings c) : cells(c) {};

Table::Table(const std::vector<Column> &columns, const std::vector<Row> &rows, const TableStyle &ts)
  : _columns(columns),
    _rows(rows),
    _cursor_style(ts.cursor_style),
    _border(ts.border),
    _table_height(ts.table_height),
    _cell_height(ts.cell_height),
    _line_seperation(ts.line_seperation) {}

Table::Table(const termui::strings &c, const std::vector<termui::strings> &r, const TableStyle &ts)
  : _columns([&] {
      std::vector<Table::Column> cols;
      cols.reserve(c.size());
      for (int i = 0; i < c.size(); i++) {
        cols.emplace_back(c.at(i));
      }
      return cols;
    }()),
    _rows([&] {
      std::vector<Table::Row> rows;
      rows.reserve(r.size());
      for (int i = 0; i < r.size(); i++) {
        rows.emplace_back(r.at(i));
      }
      return rows;
    }()),
    _cursor_style(ts.cursor_style),
    _border(ts.border),
    _table_height(ts.table_height),
    _cell_height(ts.cell_height),
    _line_seperation(ts.line_seperation) {}

Table &Table::column_width(uint col, uint w) {
  _columns.at(col).width = w;
  return *this;
}

Table &Table::table_height(uint h) {
  _table_height = h;
  return *this;
}

Table &Table::cell_height(uint h) {
  _cell_height = h;
  return *this;
}

Table &Table::line_seperation(uint ls) {
  _line_seperation = ls;
  return *this;
}

void Table::cursor_up(uint count) {
  internal_update();
  _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
  _start_line = (_cursor < _start_line) ? _cursor : _start_line;   // don't let cursor go 'above' start line
}

void Table::cursor_down(uint count) {
  internal_update();
  _cursor = (_cursor < _rows.size() - count) ? std::min(_rows.size() - 1, static_cast<size_t>(_cursor + count))
                                             : _rows.size() - 1;                                      // increment but dont let (cursor > elements.size)
  _start_line = (_cursor >= _start_line + _visible_rows) ? _cursor - _visible_rows + 1 : _start_line; // don't let cursor go 'below' bottom of visible lines
}

uint Table::get_cursor() { return _cursor; }

uint Table::colCount() { return _columns.size(); }

std::string Table::render() {
  internal_update();

  std::string outbuff;

  outbuff += curs_down(_table_height) + curs_up(_table_height);
  outbuff += Box(_table_width, _table_height, _border, Styles::faint).render();
  outbuff += curs_up(_table_height - 2) + curs_left(_table_width - 1);

  std::string header;
  for (int i = 0; i < _columns.size(); i++) {
    header += Text(_columns.at(i).title, Styles::none, _columns.at(i).width, 1).render();

    if (i != _columns.size() - 1) {
      header += " ";
    }
  }

  outbuff += header;
  outbuff += curs_down(1) + curs_left(_table_width - 2);
  outbuff += renderer.render(HorizontalLine(_table_width - 2, _border).render(), Styles::faint);
  outbuff += curs_down(1) + curs_left(_table_width - 2);

  std::string row_text; // just a buffer for this row output
  std::string cell_text;

  for (int i = _start_line; i < std::min((uint)_rows.size(), _start_line + _visible_rows); i++) {
    row_text = "";

    if (i == _cursor) {
      row_text += Text(" ", _cursor_style, _table_width - 2, _cell_height).render();
      row_text += curs_left(_table_width - 2) + curs_up(_cell_height - 1);
    }

    for (int ii = 0; ii < _columns.size(); ii++) {
      if (i == _cursor) {
        cell_text = Text(_rows.at(i).cells.at(ii), _cursor_style, _columns.at(ii).width, _cell_height).render();
      } else {
        cell_text = Text(_rows.at(i).cells.at(ii), Styles::none, _columns.at(ii).width, _cell_height).render();
      }

      row_text += cell_text;
      if (ii != _columns.size() - 1) { // if not last column
        row_text += curs_up(_cell_height - 1);
        row_text += curs_right(1); // go up if cell is multiline, right to next col
      }
    }

    outbuff += row_text;
    outbuff += curs_down(_line_seperation + 1) + curs_left(_table_width - 2); // go down (line sep), got to col 0 of table
  }

  return outbuff;
}

void Table::internal_update() {
  _visible_rows = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (_visible_rows + 1) + (_line_seperation * _visible_rows);

    if (space_used > _table_height - _overhead) {
      break;
    }
    _visible_rows++;
  }

  if (_cursor < _start_line || _start_line + _visible_rows < _cursor) {
    _start_line = _cursor;
  }

  _table_width = _columns.size() + 1;
  for (auto c : _columns) {
    _table_width += c.width;
  }
}

} // namespace termui
