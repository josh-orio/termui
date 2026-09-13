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
  FancyList &width(unsigned int w);
  FancyList &height(unsigned int h);
  FancyList &line_seperation(unsigned int ls);

  void         cursor_up(unsigned int count = 1);
  void         cursor_down(unsigned int count = 1);
  unsigned int get_cursor();

  void render();

private:
  std::vector<Element> _rows;
  termui::Style        _focus_style, _blur_style;
  unsigned int         _w, _h;
  unsigned int         _visible_rows;
  unsigned int         _start_line;
  unsigned int         _cursor;
  unsigned int         _line_spacing;

  void internal_update();
};

} // namespace termui

#endif
