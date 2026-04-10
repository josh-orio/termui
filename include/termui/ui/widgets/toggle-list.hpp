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
  ToggleList &width(uint w);
  ToggleList &height(uint h);
  ToggleList &line_seperation(uint ls);

  void cursor_up(uint count = 1);
  void cursor_down(uint count = 1);
  uint get_cursor();
  void toggle(); // toggles selection on current element
  bool getSelection(int i);

  std::string render();

private:
  termui::strings   _elements;
  std::vector<bool> _selmap;
  termui::Style     _focus_style, _blur_style;
  uint              _w, _h;
  uint              _visible_lines;
  uint              _start_line;
  uint              _cursor;
  uint              _line_spacing;

  void internal_update();
};

} // namespace termui

#endif
