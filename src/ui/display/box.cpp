#include <termui/ui/ui.hpp>

namespace termui {

Box::Box(uint w, uint h, Border border, Style style) : w(w), h(h), border(border), style(style) {}

std::string Box::render() {
  std::string outbuff;

  outbuff += border.TopLeft + repeat(border.Top, w - 2) + border.TopRight;
  outbuff += curs_left(w) + curs_down(1);

  outbuff += repeat(border.Left + repeat(" ", w - 2) + border.Right + curs_left(w) + curs_down(1), h - 2);

  outbuff += border.BottomLeft + repeat(border.Bottom, w - 2) + border.BottomRight;

  return renderer.render(outbuff, style);
}

void Box::resize(uint width, uint height) {
  w = width;
  h = height;
}

uint Box::width() { return w; }

uint Box::height() { return h; }

} // namespace termui
