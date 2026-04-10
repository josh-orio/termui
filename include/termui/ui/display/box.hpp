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
  Box(uint width, uint height, Border border, Style style = Styles::none);

  std::string render();

  void resize(uint w, uint h);
  uint width(), height();

private:
  uint           w, h;
  termui::Border border;
  Style          style;
};

} // namespace termui

#endif
