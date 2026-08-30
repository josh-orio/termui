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
  List &width(unsigned int w);
  List &height(unsigned int h);
  List &line_seperation(unsigned int ls);

  void cursor_up(unsigned int count = 1);
  void cursor_down(unsigned int count = 1);
  unsigned int get_cursor();

  void render();

private:
  termui::strings _elements;
  termui::Style   _focus_style, _blur_style;
  unsigned int            _w, _h;
  unsigned int            _visible_lines;
  unsigned int            _start_line;
  unsigned int            _cursor;
  unsigned int            _line_spacing;

  void internal_update();
};

} // namespace termui

#endif
