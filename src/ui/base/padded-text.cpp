#include <termui/ui/ui.hpp>

namespace termui {

PaddedText::PaddedText(const termui::string &str, Style style, Padding padding) : Text(str, style), _padding(padding) {}

PaddedText &PaddedText::setPadding(Padding p) {
  _padding = p;
  return *this;
}

unsigned int PaddedText::hPadding() { return static_cast<int>(_padding.left) + static_cast<int>(_padding.right); }

unsigned int PaddedText::vPadding() { return static_cast<int>(_padding.top) + static_cast<int>(_padding.bottom); }

void PaddedText::render() {
  auto w_copy = _w, h_copy = _h;

  if (_w) {
    *_w = std::max(0, static_cast<int>(*_w) - static_cast<int>(_padding.left) - static_cast<int>(_padding.right));
  }
  if (_h) {
    *_h = std::max(0, static_cast<int>(*_h) - static_cast<int>(_padding.top) - static_cast<int>(_padding.bottom));
  }

  const std::vector<std::string> formatted = layout(); // requires padding conscious w and h - fit to text still works fine if _w and _h are not set

  _w = w_copy;
  _h = h_copy;

  if (formatted.empty()) {
    return;
  }

  terminal.StyleStack(_style);
  terminal.SaveCursorPosition();

  // drawing background (padding)
  for (int i = 0; i < formatted.size() + _padding.top + _padding.bottom; i++) {
    if (i > 0) {
      terminal.CursorDown(i);
    }

    terminal.write(std::string(visible_length(formatted.at(0)) + _padding.left + _padding.right, ' '));
    terminal.RestoreCursorPosition();
  }

  terminal.CursorRight(_padding.left).CursorDown(_padding.top); // on final iteration, move cursor to where text will start

  terminal.SaveCursorPosition();

  for (int i = 0; i < formatted.size(); i++) {
    if (i > 0) {
      terminal.RestoreCursorPosition();
      terminal.CursorDown(i);
    }

    terminal.write(formatted.at(i));
  }

  // move cursor to end of padding
  terminal.CursorDown(_padding.bottom).CursorRight(_padding.right);

  terminal.StylePop();
}

} // namespace termui
