#include <termui/elements.hpp>

namespace termui {

Box::Box(int w, int h, int color) : w(w), h(h), color(color) {}

std::string Box::render() {
  std::string outbuff;

  outbuff += term::SAVE_CURSOR;

  outbuff += "╭" + horizontal_line(w - 2) + "╮"; // top line and rounded corners
  for (int i = 0; i < h - 2; i++) {
    outbuff += term::RESTORE_CURSOR + curs_down(i + 1);
    outbuff += unicode::VERTICAL + std::string(w - 2, ' ') + unicode::VERTICAL; // spaces 'fill' bgnd
  }

  outbuff += term::RESTORE_CURSOR + curs_down(h - 1);
  outbuff += "╰" + horizontal_line(w - 2) + "╯"; // bottom line and rounded corners

  if (color != clr::DEFAULT) {
    outbuff = fg_apply(outbuff, color);
  }

  return outbuff;
}

} // namespace termui
