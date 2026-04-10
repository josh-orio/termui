#include <termui/ui/ui.hpp>

namespace termui {

HorizontalLine::HorizontalLine(uint width, Border border) : w(width), border(border) {}

std::string HorizontalLine::render() {
  std::string outbuff;

  for (int i = 0; i < w; i++) {
    outbuff += border.Top;
  }

  return outbuff;
}

uint HorizontalLine::width() { return w; }

VerticalLine::VerticalLine(uint height, Border border) : h(height), border(border) {}

std::string VerticalLine::render() {
  std::string outbuff;
  std::string curs_return = curs_down(1) + curs_left(1);

  for (int i = 0; i < h; i++) {
    outbuff += border.Left;
    outbuff += curs_return;
  }

  return outbuff;
}

uint VerticalLine::height() { return h; }

} // namespace termui
