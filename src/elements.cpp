#include "elements.hpp"

namespace termui {

Box::Box(int w, int h, std::string color) : w(w), h(h), color(color) {}

std::string Box::render() {
  std::string outbuff;

  outbuff += "╭" + div_line(w - 2) + "╮"; // top line and rounded corners
  for (int i = 0; i < h - 2; i++) {
    outbuff += std::format("\e[{}B\e[{}D│", 1, w); // cursor down 1, left w & │
    outbuff += std::format("\e[{}C│", w - 2);      // cursor right w-2 & │
  }
  outbuff += std::format("\e[{}B\e[{}D", 1, w) + "╰"; // cursor down 1, left w
  outbuff += div_line(w - 2) + "╯";                   // bottom line and rounded corners

  if (!color.empty()) {
    outbuff = color + outbuff + fg::DEFAULT;
  }

  return outbuff;
}

Button::Button(int w, std::string text, std::string active_color) : w(w), text(text), active_color(active_color) {}

std::string Button::render() {
  std::string outbuff;

  if (selected) {
    outbuff += active_color;
  } else {
    outbuff += bg_color(247); // temporary 'inactive' color
  }

  outbuff += trim_str(text, w); // TODO: centrally position text in button

  outbuff += format::NONE;

  return outbuff;
}

ProgressBar::ProgressBar(int width, float completion, std::string fg_col, std::string bg_col)
    : width(width), completion(completion), fg_col(fg_col), bg_col(bg_col){};

std::string ProgressBar::render() {
  std::string outbuff;

  outbuff += fg_col;
  for (int i = 0; i < completion * width; i++) {
    outbuff += "█";
  }

  outbuff += bg_col;
  for (int i = completion * width; i < width; i++) {
    outbuff += "█";
  }

  outbuff += format::NONE;

  return outbuff;
}

} // namespace termui
