#include "elements.hpp"

namespace termui {

Button::Button(int w, std::string text, std::string active_color, std::string idle_color)
    : w(w), text(text), active_color(active_color), idle_color(idle_color) {}

std::string Button::render() {
  std::string outbuff;

  if (text.length() > w) {
    outbuff += text.substr(0, w - 1) + symbol::ELLIPSIS;

  } else if (text.length() < w) { // center the text
    int diff = w - text.length();
    int l = diff / 2;
    int r = diff - l;
    text = std::string(l, ' ') + text + std::string(r, ' ');
    outbuff += text;

  } else {
    outbuff += text;
  }

  if (selected) {
    outbuff = fg_apply(outbuff, active_color);
  } else {
    outbuff = fg_apply(outbuff, idle_color);
  }

  return outbuff;
}

} // namespace termui
