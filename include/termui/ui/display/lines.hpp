#ifndef LINES_HPP
#define LINES_HPP

#include <string>
#include <termui/core/border.hpp>
#include <termui/core/style.hpp>
#include <termui/globals.hpp>
#include <termui/util.hpp>

namespace termui {

class HorizontalLine {
public:
  HorizontalLine(uint width, Border border);

  std::string render();
  uint        width();

private:
  uint           w;
  termui::Border border;
};

class VerticalLine {
public:
  VerticalLine(uint height, Border border);

  std::string render();
  uint        height();

private:
  uint           h;
  termui::Border border;
};

} // namespace termui

#endif
