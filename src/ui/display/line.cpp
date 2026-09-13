#include <termui/ui/ui.hpp>

namespace termui {

Line::Line(Direction d, size_t l, std::string s) : direction(d), len(l) {
  if (s.empty()) {
    if (d == Direction::Horizontal) {
      symbol = "─";
    }
    else if (d == Direction::Vertical) {
      symbol = "│";
    }
    else if (d == Direction::Ascending) {
      symbol = "╱";
    }
    else { // Descending
      symbol = "╲";
    }
  }
  else {
    symbol = s;
  }
}

void Line::render() {
  for (int i = 0; i < len; i++) {
    terminal.write(symbol);

    if (i + 1 == len) {
      break; // dont even need a cursor reposition in this case so just break
    }

    if (direction == Direction::Horizontal) {
      // no need to repo cursor
    }
    else if (direction == Direction::Vertical) {
      terminal.CursorDown(1).CursorLeft(1);
    }
    else if (direction == Direction::Ascending) {
      terminal.CursorUp(1);
    }
    else { // Descending
      terminal.CursorDown(1);
    }
  }
}

} // namespace termui
