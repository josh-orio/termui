#ifndef AREA_HPP
#define AREA_HPP

#include <string>
#include <termui/core/border.hpp>
#include <termui/core/style.hpp>
#include <termui/globals.hpp>
#include <termui/util.hpp>

namespace termui {

class Area {
public:
  Area(uint width, uint height, Style style, std::string symbol = "╱");

  std::string render();
  uint        width(), height();

private:
  uint  w, h;
  Style style;

  std::string symbol;
};

} // namespace termui

#endif
