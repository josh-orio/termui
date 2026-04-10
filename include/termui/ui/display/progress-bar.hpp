#ifndef PROGRESS_HPP
#define PROGRESS_HPP

#include <string>
#include <termui/core/border.hpp>
#include <termui/core/style.hpp>
#include <termui/globals.hpp>
#include <termui/util.hpp>

namespace termui {

class ProgressBar {
public:
  ProgressBar(uint width, float decimal, Color foreground, Color background);

  std::string render();
  uint        width();

private:
  uint  w;
  float decimal;
  Color fg, bg;
};

} // namespace termui

#endif
