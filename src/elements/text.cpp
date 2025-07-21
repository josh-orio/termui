#include "elements.hpp"

namespace termui {

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

  if (copy.size() > 0) { // data remaining, didnt fit in text box, print ellipsis
    formatted[h - 1] = formatted[h - 1].substr(0, w - 1) + symbol::ELLIPSIS;
  }

  for (int i = 0; i < h; i++) {
    outbuff += formatted[i];
    if (i == h - 1) {
      continue; // continue to avoid interfering with other ui elements
    } else {
      outbuff += curs_down(1) + curs_left(w);
    }
  }

  outbuff += format::NONE;

  return outbuff;
}

} // namespace termui
