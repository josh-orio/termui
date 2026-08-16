#include <termui/ui/ui.hpp>

namespace termui {

Box::Box(uint w, uint h, Border border, Style style) : w(w), h(h), border(border), style(style) {}

void Box::render() {
  terminal.StyleStack(style);

  terminal.write(border.TopLeft + repeat(border.Top, w - 2) + border.TopRight);
  terminal.CursorLeft(w).CursorDown(1);

  for (int i = 0; i < h - 2; i++) {
    terminal.write(border.Left + repeat(" ", w - 2) + border.Right);
    terminal.CursorLeft(w).CursorDown(1);
  }

  terminal.write(border.BottomLeft + repeat(border.Top, w - 2) + border.BottomRight);

  terminal.StylePop();
}

void Box::resize(uint width, uint height) {
  w = width;
  h = height;
}

uint Box::width() { return w; }

uint Box::height() { return h; }

} // namespace termui
