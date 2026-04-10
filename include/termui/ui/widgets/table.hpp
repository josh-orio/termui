#ifndef TABLE_HPP
#define TABLE_HPP

#include <termui/core/border.hpp>
#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class Table {
public:
  struct Column {
    termui::string title;
    uint           width;

    Column(termui::string str, uint w = 1);
  };

  struct Row {
    termui::strings cells;

    Row(termui::strings c);
  };

  struct TableStyle {
    uint   table_height;
    uint   cell_height;
    uint   line_seperation;
    Style  cursor_style;
    Border border;

    TableStyle(uint table_height = 0, uint cell_height = 1, uint line_seperation = 0, Style cursor_style = Style(std::nullopt, 57),
               Border border = Borders::rounded)
      : table_height(table_height), cell_height(cell_height), line_seperation(line_seperation), cursor_style(cursor_style), border(border) {}
  };

  Table(const std::vector<Column> &columns, const std::vector<Row> &rows, const TableStyle &ts = {});
  Table(const termui::strings &columns, const std::vector<termui::strings> &rows, const TableStyle &ts = {});

  Table &column_width(uint col, uint w);
  Table &table_height(uint h);
  Table &cell_height(uint h);
  Table &line_seperation(uint ls);

  void cursor_up(uint count = 1);
  void cursor_down(uint count = 1);
  uint get_cursor();
  uint colCount();

  std::string render();

private:
  std::vector<Column> _columns;
  std::vector<Row>    _rows;
  Style               _cursor_style;
  Border              _border = Borders::rounded;
  uint                _table_height;
  uint                _table_width;
  uint                _cell_height;
  const uint          _overhead = 4; // number of lines reserved for header & footer
  uint                _visible_rows; // number of table rows that fit the h restraint
  uint                _start_line;   // index value of first visible row
  uint                _cursor;
  uint                _line_seperation;

  void internal_update();
};

} // namespace termui

#endif
