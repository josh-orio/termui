#include "elements.hpp"

namespace termui {

Button::Button() = default;
Button::Button(const std::string &rT, int w, int active_color, int idle_color)
    : text(std::make_shared<std::string>(rT)), w(w), active_color(active_color), idle_color(idle_color) {}
Button::Button(std::string &&lT, int w, int active_color, int idle_color)
    : text(std::make_shared<std::string>(std::move(lT))), w(w), active_color(active_color), idle_color(idle_color) {}
Button::Button(std::shared_ptr<std::string> sharedT, int w, int active_color, int idle_color)
    : text(std::move(sharedT)), w(w), active_color(active_color), idle_color(idle_color) {}

const std::string &Button::getText() const { return *text; }
std::string &Button::getText() { return *text; }

std::string Button::render() {
  std::string outbuff;

  std::string txt = (*text).data();

  if (txt.length() > w) {
    outbuff += txt.substr(0, w - 1) + unicode::ELLIPSIS;

  } else if (txt.length() < w) { // center the text
    int diff = w - txt.length();
    int l = diff / 2;
    int r = diff - l;
    outbuff += std::string(l, ' ') + txt + std::string(r, ' ');

  } else {
    outbuff += txt;
  }

  if (selected) {
    outbuff = bg_apply(outbuff, active_color);
  } else {
    outbuff = bg_apply(outbuff, idle_color);
  }

  return outbuff;
}

} // namespace termui
