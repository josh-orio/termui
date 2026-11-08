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
  bool has_w = _w.has_value(), has_h = _h.has_value();
  int  w = *_w, h = *_h;

  if (_w == 0 || _h == 0) {
    return;
  }

  terminal.StyleStack(_style);

  // drawing background (padding)
  // save/restore rather than CursorLeft(w): a row ending in the last column doesn't advance the cursor
  for (int i = h; i > 0; --i) {
    terminal.SaveCursorPosition();
    terminal.write(std::string(w, ' '));
    terminal.RestoreCursorPosition();

    if (i > 1) {
      terminal.CursorDown(1);
    }
    else {
      terminal.CursorRight(_padding.left).CursorUp(h - _padding.top - 1); // on final iteration, move cursor to where text will start
    }
  }

  auto w_copy = w, h_copy = h;

  *_w = std::max(0, static_cast<int>(w) - static_cast<int>(_padding.left) - static_cast<int>(_padding.right));
  *_h = std::max(0, static_cast<int>(h) - static_cast<int>(_padding.top) - static_cast<int>(_padding.bottom));
  Text::render(); // call the method from the base class
  *_w = w_copy;
  *_h = h_copy;

  // move cursor to end of padding
  terminal.CursorDown(_padding.bottom).CursorRight(_padding.right);

  terminal.StylePop();
}

} // namespace termui
