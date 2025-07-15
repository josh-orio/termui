#include "elements.hpp"

namespace termui {

Box::Box(int w, int h, std::string color) : w(w), h(h), color(color) {}

std::string Box::render() {
  std::string outbuff;

  outbuff += "╭" + div_line(w - 2) + "╮"; // top line and rounded corners
  for (int i = 0; i < h - 2; i++) {
    outbuff += std::format("\e[{}B\e[{}D│", 1, w); // cursor down 1, left w & │
    outbuff += std::format("\e[{}C│", w - 2);      // cursor right w-2 & │
  }
  outbuff += std::format("\e[{}B\e[{}D", 1, w) + "╰"; // cursor down 1, left w
  outbuff += div_line(w - 2) + "╯";                   // bottom line and rounded corners

  if (!color.empty()) {
    outbuff = color + outbuff + fg::DEFAULT;
  }

  return outbuff;
}

} // namespace termui
