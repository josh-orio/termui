#include <termui/ui/ui.hpp>

namespace termui {

Button::Button(const termui::string &text, Style focus_style, Style blur_style) : _text(text), _focus_style(focus_style), _blur_style(blur_style) {}

Button &Button::focus_style(Style s) {
  _focus_style = s;
  return *this;
}

Button &Button::blur_style(Style s) {
  _blur_style = s;
  return *this;
}

Button &Button::width(unsigned int w) {
  _w = w;
  return *this;
}

Button &Button::focus() {
  _selected = true;
  return *this;
}

Button &Button::blur() {
  _selected = false;
  return *this;
}

Button &Button::setFocus(bool x) {
  _selected = x;
  return *this;
}

void Button::render() {
  std::string txt = _text;

  if (_selected) {
    terminal.StyleStack(_focus_style);

  } else {
    terminal.StyleStack(_blur_style);
  }

  if (txt.length() > _w) {
    terminal.write(txt.substr(0, _w - 1) + unicode::ELLIPSIS);

  } else if (txt.length() < _w) { // center the text
    int diff = _w - txt.length();
    int l = diff / 2;
    int r = diff - l;

    terminal.write(std::string(l, ' ') + txt + std::string(r, ' '));

  } else {
    terminal.write(txt);
  }

  terminal.StylePop();
}

} // namespace termui
