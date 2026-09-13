#ifndef BOX_HPP
#define BOX_HPP

#include <string>
#include <termui/core/border.hpp>
#include <termui/core/style.hpp>
#include <termui/globals.hpp>
#include <termui/util.hpp>

namespace termui {

class Box {
public:
  Box(unsigned int width, unsigned int height, Border border, Style style = Styles::none);

  void render();

  void         resize(unsigned int w, unsigned int h);
  unsigned int width(), height();

private:
  unsigned int   w, h;
  termui::Border border;
  Style          style;
};

} // namespace termui

#endif
