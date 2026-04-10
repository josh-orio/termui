#include <termui/ui/ui.hpp>

namespace termui {

Pager::Pager(const termui::string &str, const Style &style, uint width, uint height) : Text(str, style, width, height), _cursor(0) {}

std::string Pager::render() {
  if (_w == 0 || _h == 0) {
    return "";
  }

  std::string outbuff;

  std::vector<std::string> formatted;
  std::string              copy = _text;
  size_t                   next = 0;

  while (copy.size() > 0) {
    // clang-format off
      next = std::min({
      copy.size(),

      max_visible_length(copy, _w),

      copy.find('\n') == std::string::npos
        ? copy.size()
        : copy.find('\n'),

      (copy.rfind(' ', max_visible_length(copy, _w)) == std::string::npos || visible_length(copy) <= _w)
        ? copy.size()
        : copy.rfind(' ', max_visible_length(copy, _w))
      });
    // clang-format on

    formatted.push_back(std::string(copy.begin(), copy.begin() + next));         // make substring of <=w printed symbols
    formatted.back() += std::string(_w - visible_length(formatted.back()), ' '); // add spacing to fill w

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
    if (formatted.size() > 0) {
      _cursor = formatted.size() - 1;

    } else {
      _cursor = 0;
    }
  }

  while (formatted.size() < _h) {
    formatted.push_back(std::string(_w, ' '));
  }

  for (int i = _cursor; i < _cursor + _h; i++) {
    if (i < formatted.size()) {
      outbuff += formatted.at(i);

    } else {
      outbuff += std::string(_w, ' ');
    }

    outbuff += curs_left(_w) + curs_down(1);
  }

  return renderer.render(outbuff, _style);
}

// uint Pager::width() { return w; }

// uint Pager::height() { return h; }

// void Pager::resize(uint width, uint height) {
//   w = width;
//   h = height;
// }

Pager &Pager::cursor_up(uint count) {
  _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
  return *this;
}
Pager &Pager::cursor_down(uint count) {
  _cursor += count; // no checking is done here - the value is clamped in render() so that the bottom piece of text is always visible
  return *this;
}

uint Pager::get_cursor() { return _cursor; }

} // namespace termui
