#include <termui/ui/ui.hpp>

namespace termui {

Line::Line(Direction d, size_t l, std::string s) : _direction(d), _len(l) {
  if (s.empty()) {
    if (d == Direction::Horizontal) {
      _symbol = "─";
    }
    else if (d == Direction::Vertical) {
      _symbol = "│";
    }
    else if (d == Direction::Ascending) {
      _symbol = "╱";
    }
    else { // Descending
      _symbol = "╲";
    }
  }
  else {
    _symbol = s;
  }
}

Line &Line ::style(Style s) {
  _style = s;
  return *this;
}

void Line::render() {
  terminal.StyleStack(_style);

  for (int i = 0; i < _len; i++) {
    terminal.write(_symbol);

    if (i + 1 == _len) {
      break; // dont even need a cursor reposition in this case so just break
    }

    if (_direction == Direction::Horizontal) {
      // no need to repo cursor
    }
    else if (_direction == Direction::Vertical) {
      terminal.CursorDown(1).CursorLeft(1);
    }
    else if (_direction == Direction::Ascending) {
      terminal.CursorUp(1);
    }
    else { // Descending
      terminal.CursorDown(1);
    }
  }

  terminal.StylePop();
}

} // namespace termui
