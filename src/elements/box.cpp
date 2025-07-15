#include "elements.hpp"

namespace termui {

Box::Box(int w, int h, std::string color) : w(w), h(h), color(color) {}

std::string Box::render() {
  std::string outbuff;

  outbuff += "╭" + div_line(w - 2) + "╮"; // top line and rounded corners
  for (int i = 0; i < h - 2; i++) {
    outbuff += curs_down(1) + curs_left(w) + "│";
    outbuff += curs_right(w - 2) + "│";
  }
  outbuff += curs_down(1) + curs_left(w);
  outbuff += "╰" + div_line(w - 2) + "╯"; // bottom line and rounded corners

  if (!color.empty()) {
    outbuff = color + outbuff + fg::DEFAULT;
  }

  return outbuff;
}

} // namespace termui
