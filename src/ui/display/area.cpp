#include <termui/ui/ui.hpp>

namespace termui {

Area::Area(unsigned int w, unsigned int h, Style style, std::string symbol) : w(w), h(h), style(style), symbol(symbol) {}

void Area::render() {
  terminal.StyleStack(style);

  for (int i = 0; i < h; i++) {
    for (int ii = 0; ii < w; ii++) {
      terminal.write(symbol);
    }
    terminal.CursorDown(1).CursorLeft(w);
  }

  terminal.StylePop();
}

} // namespace termui
