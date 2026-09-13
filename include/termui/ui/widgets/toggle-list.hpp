#ifndef TOGGLELIST_HPP
#define TOGGLELIST_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class ToggleList {
public:
  ToggleList(const termui::strings &strs, Style focus_style = {}, Style blur_style = {});

  ToggleList &focus_style(Style s);
  ToggleList &blur_style(Style s);
  ToggleList &width(unsigned int w);
  ToggleList &height(unsigned int h);
  ToggleList &line_seperation(unsigned int ls);

  void         cursor_up(unsigned int count = 1);
  void         cursor_down(unsigned int count = 1);
  unsigned int get_cursor();
  void         toggle(); // toggles selection on current element
  bool         getSelection(int i);

  void render();

private:
  termui::strings   _elements;
  std::vector<bool> _selmap;
  termui::Style     _focus_style, _blur_style;
  unsigned int      _w, _h;
  unsigned int      _visible_lines;
  unsigned int      _start_line;
  unsigned int      _cursor;
  unsigned int      _line_spacing;

  void internal_update();
};

} // namespace termui

#endif
