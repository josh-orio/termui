#include <termui/ui/ui.hpp>

namespace termui {

Area::Area(uint w, uint h, Style style, std::string symbol) : w(w), h(h), style(style), symbol(symbol) {}

std::string Area::render() {
  std::string outbuff;
  std::string curs_return = curs_down(1) + curs_left(w);

  for (int i = 0; i < h; i++) {
    for (int ii = 0; ii < w; ii++) {
      outbuff += symbol;
    }
    outbuff += curs_return;
  }

  return renderer.render(outbuff, style);
}

} // namespace termui
