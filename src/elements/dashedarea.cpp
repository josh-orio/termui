#include "elements.hpp"

namespace termui {

DashedArea::DashedArea(int w, int h, int col) : w(w), h(h), col(col) {}

std::string DashedArea::render() {
  std::string outbuff;
  std::string curs_return = curs_down(1) + curs_left(w);
  
  for (int i = 0; i < h; i++) {
    for (int ii = 0; ii < w; ii++) {
      outbuff += "╱";
    }
    outbuff += curs_return;
  }

  if (col != clr::DEFAULT) {
    outbuff = fg_apply(outbuff, col);
  }

  return outbuff;
}

} // namespace termui
