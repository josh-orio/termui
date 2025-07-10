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

Button::Button(int w, std::string text, std::string active_color, std::string idle_color)
    : w(w), text(text), active_color(active_color), idle_color(idle_color) {}

std::string Button::render() {
  std::string outbuff;

  if (selected) {
    outbuff += active_color;
  } else {
    outbuff += idle_color;
  }

  if (text.length() > w) {
    outbuff += text.substr(0, w - 3) + "...";
  } else if (text.length() < w) {
    int diff = w - text.length();
    int l = diff / 2;
    int r = diff - l;
    text = std::string(l, ' ') + text + std::string(r, ' ');
    outbuff += text;
  } else {
    outbuff += text;
  }

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

Text::Text(std::string data, int w, int h, std::string fg_col, std::string bg_col)
    : data(data), w(w), h(h), fg_col(fg_col), bg_col(bg_col){};

std::string Text::render() {
  std::string outbuff;

  outbuff += fg_col + bg_col;

  std::vector<std::string> formatted;
  std::string copy = data;

  for (int i = 0; i < h; i++) {
    int next = 0;
    if (copy.find('\n') == std::string::npos) {
      next = std::min({(int)copy.size(), w});
    } else {
      next = std::min({(int)copy.size(), w, (int)copy.find('\n')});
    }

    formatted.push_back(std::string(copy.begin(), copy.begin() + next) + std::string(w - next, ' '));

    if (copy.begin() + next + 1 < copy.end()) {
      if (copy[next] == '\n') {
        copy = std::string(copy.begin() + next + 1, copy.end());
      } else {
        copy = std::string(copy.begin() + next, copy.end());
      }
    } else {
      copy = "";
    }
  }

  if (copy.size() > 0) { // data remaining, didnt fit in text box, print ...
    formatted[h - 1] = formatted[h - 1].substr(0, w - 3) + "...";
  }

  for (int i = 0; i < h; i++) {
    outbuff += formatted[i];
    if (i == h - 1) {
      continue; // continue to avoid interfering with other ui elements
    } else {
      outbuff += std::format("\e[{}B\e[{}D", 1, w); // cursor down 1, left w
    }
  }

  outbuff += format::NONE;

  return outbuff;
}

} // namespace termui
