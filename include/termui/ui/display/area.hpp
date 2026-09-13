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
  Area(unsigned int width, unsigned int height, Style style, std::string symbol = "╱");

  void         render();
  unsigned int width(), height();

private:
  unsigned int w, h;
  Style        style;

  std::string symbol;
};

} // namespace termui

#endif
