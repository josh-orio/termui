#include "elements.hpp"

namespace termui {

Text::Text() = default;
Text::Text(const termui::string &t, int w, int h, int fg_col, int bg_col) : text(t), w(w), h(h), fg_col(fg_col), bg_col(bg_col) {}
Text::Text(const std::string &t, int w, int h, int fg_col, int bg_col) : text(t), w(w), h(h), fg_col(fg_col), bg_col(bg_col) {}
Text::Text(std::shared_ptr<std::string> &t, int w, int h, int fg_col, int bg_col) : text(t), w(w), h(h), fg_col(fg_col), bg_col(bg_col) {}

std::string Text::render() {
  std::string outbuff;

  std::vector<std::string> formatted;
  std::string copy = text.text();

  for (int i = 0; i < h; i++) {
    int next = 0;
    if (copy.find('\n') == std::string::npos) {
      next = std::min({copy.size(), max_visible_length(copy, w)});
    } else {
      next = std::min({copy.size(), max_visible_length(copy, w), copy.find('\n')});
    }

    formatted.push_back(std::string(copy.begin(), copy.begin() + next));        // make substring of <=w printed symbols
    formatted.back() += std::string(w - visible_length(formatted.back()), ' '); // add spacing to fill w

    if (copy.begin() + next < copy.end()) {
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
    formatted.back() = std::string(formatted.back().begin(), formatted.back().end() - 1) + unicode::ELLIPSIS; // holy method.method
  }

  for (int i = 0; i < h; i++) {
    outbuff += formatted[i];
    if (i == h - 1) {
      continue; // continue to avoid interfering with other ui elements
    } else {
      outbuff += curs_down(1) + curs_left(w);
    }
  }

  outbuff = fg_apply(outbuff, fg_col);
  outbuff = bg_apply(outbuff, bg_col);

  return outbuff;
}

} // namespace termui
