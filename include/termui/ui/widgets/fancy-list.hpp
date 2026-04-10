#ifndef FANCYLIST_HPP
#define FANCYLIST_HPP

#include <termui/core/str.hpp>
#include <termui/core/style.hpp>

namespace termui {

class FancyList {
public:
  struct Element {
    termui::string title, description;

    Element(const termui::string &title, const termui::string &description) : title(title), description(description) {}
  };

  FancyList(const std::vector<Element> &rows, Style focus_style = {}, Style blur_style = {});

  FancyList &focus_style(Style s);
  FancyList &blur_style(Style s);
  FancyList &width(uint w);
  FancyList &height(uint h);
  FancyList &line_seperation(uint ls);

  void cursor_up(uint count = 1);
  void cursor_down(uint count = 1);
  uint get_cursor();

  std::string render();

private:
  std::vector<Element> _rows;
  termui::Style        _focus_style, _blur_style;
  uint                 _w, _h;
  uint                 _visible_rows;
  uint                 _start_line;
  uint                 _cursor;
  uint                 _line_spacing;

  void internal_update();
};

} // namespace termui

#endif
