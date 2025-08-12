#include "elements.hpp"

namespace termui {

Text::Text() = default;
Text::Text(const std::string &rT, int w, int h, int fg_col, int bg_col) : text(std::make_shared<std::string>(rT)), w(w), h(h), fg_col(fg_col), bg_col(bg_col) {}
Text::Text(std::string &&lT, int w, int h, int fg_col, int bg_col)
    : text(std::make_shared<std::string>(std::move(lT))), w(w), h(h), fg_col(fg_col), bg_col(bg_col) {}
Text::Text(std::shared_ptr<std::string> sharedT, int w, int h, int fg_col, int bg_col) : text(std::move(sharedT)), w(w), h(h), fg_col(fg_col), bg_col(bg_col) {}

std::shared_ptr<std::string> Text::share() const { return text; }
// share function shares a pointer to the content
// for this class only text needs to be shared, but more advanced classes may need multiple share function

const std::string &Text::getText() const { return *text; }
std::string &Text::getText() { return *text; }

std::string Text::render() {
  std::string outbuff;

  std::vector<std::string> formatted;
  std::string copy = (*text).data();

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
    formatted[h - 1] = formatted[h - 1].substr(0, w - 1) + unicode::ELLIPSIS;
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
