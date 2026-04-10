#include <termui/ui/ui.hpp>

namespace termui {

PaddedText::PaddedText(const termui::string &str, Style style, Padding padding) : Text(str, style), _padding(padding) {}

uint PaddedText::hPadding() { return static_cast<int>(_padding.left) + static_cast<int>(_padding.right); }
uint PaddedText::vPadding() { return static_cast<int>(_padding.top) + static_cast<int>(_padding.bottom); }

std::string PaddedText::render() {
  if (_w == 0 || _h == 0) {
    return "";
  }

  std::string outbuff;

  // drawing background (padding)
  for (int i = _h; i > 0; --i) {
    outbuff += std::string(_w, ' ');

    if (i > 1) {
      outbuff += curs_left(_w);
      outbuff += curs_down(1);

    } else {
      outbuff += curs_left(_w - _padding.left); // on final iteration, move cursor to where text will start
      outbuff += curs_up(_h - _padding.top - 1);
    }
  }

  auto w_copy = _w, h_copy = _h;

  _w = std::max(0, static_cast<int>(_w) - static_cast<int>(_padding.left) - static_cast<int>(_padding.right));
  _h = std::max(0, static_cast<int>(_h) - static_cast<int>(_padding.top) - static_cast<int>(_padding.bottom));
  outbuff += Text::render(); // call the method from base class
  _w = w_copy;
  _h = h_copy;

  // move cursor to end of padding
  outbuff += curs_down(_padding.bottom) + curs_right(_padding.right);

  return renderer.render(outbuff, _style);
}

} // namespace termui
