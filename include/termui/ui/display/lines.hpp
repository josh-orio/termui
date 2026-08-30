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
  HorizontalLine(unsigned int width, Border border);

  void render();
  unsigned int width();

private:
  unsigned int           w;
  termui::Border border;
};

class VerticalLine {
public:
  VerticalLine(unsigned int height, Border border);

  void render();
  unsigned int height();

private:
  unsigned int           h;
  termui::Border border;
};

} // namespace termui

#endif
