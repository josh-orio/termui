#include <termui/ui/ui.hpp>

#include <algorithm>

namespace termui {

Text::Text(const termui::string &str, const Style &style, uint width, uint height) : _text(str), _style(style), _w(width), _h(height) {}

Text &Text::style(const Style &s) {
  _style = s;
  return *this;
}

Text &Text::align(const Alignment &a) {
  _align = a;
  return *this;
}

Text &Text::width(uint w) {
  _w = w;
  return *this;
}

Text &Text::height(uint h) {
  _h = h;
  return *this;
}

Text &Text::size(uint w, uint h) {
  _w = w;
  _h = h;
  return *this;
}

std::string Text::render() {
  if (_w == 0 || _h == 0) {
    return "";
  }

  std::vector<std::string> formatted;
  std::string              copy = _text;
  size_t                   next = 0;

  for (int i = 0; i < _h; i++) {
    // clang-format off
    next = std::min({
      copy.size(),
      
      max_visible_length(copy, _w),
      
      copy.find('\n'),
      
      (copy.rfind(' ', max_visible_length(copy, _w)) == std::string::npos || visible_length(copy) <= _w)
        ? copy.size()
        : copy.rfind(' ', max_visible_length(copy, _w))
    });
    // clang-format on

    formatted.push_back(std::string(copy.begin(), copy.begin() + next)); // make substring of <=w printed symbols

    if (copy.begin() + next >= copy.end()) {
      copy = "";

    } else {
      if (copy[next] == '\n' || copy[next] == ' ') { // special handling for these two chars as they will cause infinite loops otherwise
        copy = std::string(copy.begin() + next + 1, copy.end());
      } else {
        copy = std::string(copy.begin() + next, copy.end());
      }
    }
  }

  while (formatted.size() < +_h) { // add 'overrun' strings
    formatted.push_back(std::string(_w, ' '));
  }

  // align text
  for (int i = 0; i < formatted.size(); i++) {
    if (formatted.at(i).size() < _w) {

      if (_align == Alignment::Left) {
        formatted.at(i) += std::string(_w - visible_length(formatted.at(i)), ' ');

      } else if (_align == Alignment::Center) {
        formatted.at(i) = std::string((_w - visible_length(formatted.at(i))) / 2, ' ') + formatted.at(i);
        formatted.at(i) += std::string(_w - visible_length(formatted.at(i)), ' ');

      } else /* _align == Right */ {
        formatted.at(i) = std::string(_w - visible_length(formatted.at(i)), ' ') + formatted.at(i);
      }
    }
  }

  if (copy.size() > 0) { // data remaining, didnt fit in text box, print ellipsis
    formatted.back() = std::string(formatted.back().begin(), formatted.back().end() - 1) + unicode::ELLIPSIS;
  }

  std::string outbuff;

  for (int i = 0; i < _h; i++) {
    outbuff += formatted.at(i);

    if (i + 1 < _h) {
      outbuff += curs_left(_w) + curs_down(1);
    }
  }

  return renderer.render(outbuff, _style);
}

} // namespace termui
