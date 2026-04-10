#ifndef LIST_HPP
#define LIST_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class List {
public:
  List(const termui::strings &strs, Style focus_style = {}, Style blur_style = {});

  List &focus_style(Style s);
  List &blur_style(Style s);
  List &width(uint w);
  List &height(uint h);
  List &line_seperation(uint ls);

  void cursor_up(uint count = 1);
  void cursor_down(uint count = 1);
  uint get_cursor();

  std::string render();

private:
  termui::strings _elements;
  termui::Style   _focus_style, _blur_style;
  uint            _w, _h;
  uint            _visible_lines;
  uint            _start_line;
  uint            _cursor;
  uint            _line_spacing;

  void internal_update();
};

} // namespace termui

#endif
