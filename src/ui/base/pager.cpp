#include <termui/ui/ui.hpp>

#include <algorithm>

namespace termui {

Pager::Pager(const termui::string &str, const Style &style, uint width, uint height) : Text(str, style, width, height), _cursor(0) {}

Pager &Pager::cursor_up(uint count) {
  _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
  return *this;
}
Pager &Pager::cursor_down(uint count) {
  _cursor += count; // no checking is done here - the value is clamped in render() so that the bottom piece of text is always visible
  return *this;
}

uint Pager::get_cursor() { return _cursor; }

std::string Pager::render() {
  if (_w == 0 || _h == 0) {
    return "";
  }

  std::vector<std::string> formatted;
  std::string              copy = _text;
  size_t                   next = 0;

  while (copy.size() > 0) {
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

    if (copy.begin() + next < copy.end()) {
      if (copy[next] == '\n' || copy[next] == ' ') { // special handling for these two chars as they will cause infinite loops otherwise
        copy = std::string(copy.begin() + next + 1, copy.end());
      } else {
        copy = std::string(copy.begin() + next, copy.end());
      }
    } else {
      copy = "";
    }
  }

  if (_cursor >= formatted.size()) {
    _cursor = std::max(size_t(0), formatted.size() - 1);
  }

  while (formatted.size() < _cursor + _h) { // add 'overrun' strings
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

  std::string outbuff;

  for (int i = _cursor; i < _cursor + _h; i++) {
    outbuff += formatted.at(i);

    if (i + 1 < _h) {
      outbuff += curs_left(_w) + curs_down(1);
    }
  }

  return renderer.render(outbuff, _style);
}

} // namespace termui
