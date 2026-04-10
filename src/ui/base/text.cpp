#include <termui/ui/ui.hpp>

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

// Text &Text::cursor_up(uint count) {
//   _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
//   return *this;
// }

// Text &Text::cursor_down(uint count) {
//   _cursor += count; // no checking is done here - the value is clamped in render() so that the bottom piece of text is always visible
//   return *this;
// }

// uint Text::get_cursor() { return _cursor; }

std::string Text::render() {
  if (_w == 0 || _h == 0) {
    return "";
  }

  std::string outbuff;

  std::vector<std::string> formatted;
  std::string              copy = _text; //.text();

  for (int i = 0; i < _h; i++) {
    int next = 0;
    if (copy.find('\n') == std::string::npos) {
      next = std::min({copy.size(), max_visible_length(copy, _w)});
    } else {
      next = std::min({copy.size(), max_visible_length(copy, _w), copy.find('\n')});
    }

    formatted.push_back(std::string(copy.begin(), copy.begin() + next));         // make substring of <=w printed symbols
    formatted.back() += std::string(_w - visible_length(formatted.back()), ' '); // add spacing to fill w

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
    formatted.back() = std::string(formatted.back().begin(), formatted.back().end() - 1) + unicode::ELLIPSIS;
  }

  for (int i = 0; i < _h; i++) {
    outbuff += formatted[i];
    if (i == _h - 1) {
      continue; // continue to avoid interfering with other ui elements
    } else {
      outbuff += curs_down(1) + curs_left(_w);
    }
  }

  return renderer.render(outbuff, _style);
}

} // namespace termui
