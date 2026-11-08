#include <termui/ui/ui.hpp>

namespace termui {

Pager::Pager(const termui::string &str, const Style &style, unsigned int width, unsigned int height) : Text(str, style, width, height), _cursor(0) {}

Pager &Pager::cursor_up(unsigned int count) {
  _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
  return *this;
}
Pager &Pager::cursor_down(unsigned int count) {
  _cursor += count; // no checking is done here - the value is clamped in render() so that the bottom piece of text is always visible
  return *this;
}

unsigned int Pager::get_cursor() { return _cursor; }

void Pager::render() {
  bool has_w = _w.has_value(), has_h = _h.has_value();
  int  w = *_w, h = *_h;

  if ((has_w && w == 0) || (has_h && h == 0)) {
    return;
  }

  std::vector<std::string> formatted;
  std::string              copy = _text;
  size_t                   next = 0;

  while (copy.size() > 0) {
    // clang-format off
    next = std::min({
      copy.size(),
      
      max_visible_length(copy, w),
      
      copy.find('\n'),
      
      (copy.rfind(' ', max_visible_length(copy, w)) == std::string::npos || visible_length(copy) <= w)
        ? copy.size()
        : copy.rfind(' ', max_visible_length(copy, w))
    });
    // clang-format on

    formatted.push_back(std::string(copy.begin(), copy.begin() + next)); // make substring of <=w printed symbols

    if (copy.begin() + next < copy.end()) {
      if (copy[next] == '\n' || copy[next] == ' ') { // special handling for these two chars as they will cause infinite loops otherwise
        copy = std::string(copy.begin() + next + 1, copy.end());
      }
      else {
        copy = std::string(copy.begin() + next, copy.end());
      }
    }
    else {
      copy = "";
    }
  }

  if (_cursor >= formatted.size()) {
    _cursor = std::max(size_t(0), formatted.size() - 1);
  }

  while (formatted.size() < _cursor + h) { // add 'overrun' strings
    formatted.push_back(std::string(w, ' '));
  }

  // align text
  for (int i = 0; i < formatted.size(); i++) {
    if (formatted.at(i).size() < w) {

      if (_align == Alignment::Left) {
        formatted.at(i) += std::string(w - visible_length(formatted.at(i)), ' ');
      }
      else if (_align == Alignment::Center) {
        formatted.at(i) = std::string((w - visible_length(formatted.at(i))) / 2, ' ') + formatted.at(i);
        formatted.at(i) += std::string(w - visible_length(formatted.at(i)), ' ');
      }
      else /* _align == Right */ {
        formatted.at(i) = std::string(w - visible_length(formatted.at(i)), ' ') + formatted.at(i);
      }
    }
  }

  terminal.StyleStack(_style);
  terminal.SaveCursorPosition();

  for (int i = _cursor; i < _cursor + h; i++) {
    if (i > _cursor) {
      terminal.RestoreCursorPosition();
      terminal.CursorDown(i - _cursor);
    }

    terminal.write(formatted.at(i));
  }

  terminal.StylePop();
}

} // namespace termui
