#include <termui/ui/ui.hpp>

namespace termui {

Area::Area(unsigned int w, unsigned int h, Style style, std::string symbol) : w(w), h(h), style(style), symbol(symbol) {}

void Area::render() {
  terminal.StyleStack(style);

  for (int i = 0; i < h; i++) {
    terminal.SaveCursorPosition(); // rather than CursorLeft(w): a row ending in the last column doesn't advance the cursor
    for (int ii = 0; ii < w; ii++) {
      terminal.write(symbol);
    }
    terminal.RestoreCursorPosition();
    terminal.CursorDown(1);
  }

  terminal.StylePop();
}

} // namespace termui
