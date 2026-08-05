#include <termui/ui/ui.hpp>

namespace termui {

Line::Line(Direction d, size_t l, std::string s) : direction(d), len(l) {
  if (s.empty()) {
    if (d == Direction::Horizontal) {
      symbol = "─";

    } else if (d == Direction::Vertical) {
      symbol = "│";

    } else if (d == Direction::Ascending) {
      symbol = "╱";

    } else { // Descending
      symbol = "╲";
    }

  } else {
    symbol = s;
  }
}

std::string Line::render() {
  std::string buff;

  for (int i = 0; i < len; i++) {
    buff += symbol;

    if (i + 1 == len) {
      break; // dont even need a cursor reposition in this case so just break
    }

    if (direction == Direction::Horizontal) {
      buff += "";

    } else if (direction == Direction::Vertical) {
      buff += curs_down(1) + curs_left(1);

    } else if (direction == Direction::Ascending) {
      buff += curs_up(1);

    } else { // Descending
      buff += curs_down(1);
    }
  }

  return buff;
}

} // namespace termui
