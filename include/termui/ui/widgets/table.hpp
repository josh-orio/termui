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
    unsigned int   width;

    Column(termui::string str, unsigned int w = 1);
  };

  struct Row {
    termui::strings cells;

    Row(termui::strings c);
  };

  struct TableStyle {
    unsigned int table_height;
    unsigned int cell_height;
    unsigned int line_seperation;
    Style        cursor_style;
    Border       border;

    TableStyle(unsigned int table_height = 0, unsigned int cell_height = 1, unsigned int line_seperation = 0, Style cursor_style = Style(Color::Inherit(), 57),
               Border border = Borders::rounded)
      : table_height(table_height), cell_height(cell_height), line_seperation(line_seperation), cursor_style(cursor_style), border(border) {}
  };

  Table(const std::vector<Column> &columns, const std::vector<Row> &rows, const TableStyle &ts = {});
  Table(const termui::strings &columns, const std::vector<termui::strings> &rows, const TableStyle &ts = {});

  Table &column_width(unsigned int col, unsigned int w);
  Table &table_height(unsigned int h);
  Table &cell_height(unsigned int h);
  Table &line_seperation(unsigned int ls);

  void         cursor_up(unsigned int count = 1);
  void         cursor_down(unsigned int count = 1);
  unsigned int get_cursor();
  unsigned int colCount();

  void render();

private:
  std::vector<Column> _columns;
  std::vector<Row>    _rows;
  Style               _cursor_style;
  Border              _border = Borders::rounded;
  unsigned int        _table_height;
  unsigned int        _table_width;
  unsigned int        _cell_height;
  const unsigned int  _overhead = 4; // number of lines reserved for header & footer
  unsigned int        _visible_rows; // number of table rows that fit the h restraint
  unsigned int        _start_line;   // index value of first visible row
  unsigned int        _cursor;
  unsigned int        _line_seperation;

  void internal_update();
};

} // namespace termui

#endif
