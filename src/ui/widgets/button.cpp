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

Button &Button::width(uint w) {
  _w = w;
  return *this;
}

void Button::focus() { _selected = true; }

void Button::blur() { _selected = false; }

std::string Button::render() {
  std::string outbuff;

  std::string txt = _text; //.text();

  if (txt.length() > _w) {
    outbuff += txt.substr(0, _w - 1) + unicode::ELLIPSIS;

  } else if (txt.length() < _w) { // center the text
    int diff = _w - txt.length();
    int l = diff / 2;
    int r = diff - l;
    outbuff += std::string(l, ' ') + txt + std::string(r, ' ');

  } else {
    outbuff += txt;
  }

  if (_selected) {
    return renderer.render(outbuff, _focus_style);

  } else {
    return renderer.render(outbuff, _blur_style);
  }
}

} // namespace termui
