#include <termui/ui/ui.hpp>

namespace termui {

ToggleList::ToggleList(const termui::strings &strs, Style focus_style, Style blur_style)
  : _elements(strs), _selmap(_elements.size()), _focus_style(focus_style), _blur_style(blur_style), _start_line(0), _cursor(0) {}

ToggleList &ToggleList::focus_style(Style s) {
  _focus_style = s;
  return *this;
}

ToggleList &ToggleList::blur_style(Style s) {
  _blur_style = s;
  return *this;
}

ToggleList &ToggleList::width(uint w) {
  _w = w;
  return *this;
}

ToggleList &ToggleList::height(uint h) {
  _h = h;
  return *this;
}

ToggleList &ToggleList::line_seperation(uint ls) {
  _line_spacing = ls;
  return *this;
}

void ToggleList::cursor_up(uint count) {
  internal_update();
  _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
  _start_line = (_cursor < _start_line) ? _cursor : _start_line;   // don't let cursor go 'above' start line
}

void ToggleList::cursor_down(uint count) {
  internal_update();
  _cursor = (_cursor < _elements.size() - count) ? std::min(_elements.size() - 1, static_cast<size_t>(_cursor + count))
                                                 : _elements.size() - 1;                                // increment but dont let (cursor > elements.size)
  _start_line = (_cursor >= _start_line + _visible_lines) ? _cursor - _visible_lines + 1 : _start_line; // don't let cursor go 'below' bottom of visible lines
}

uint ToggleList::get_cursor() { return _cursor; }

void ToggleList::toggle() { _selmap[_cursor] = !_selmap[_cursor]; }

bool ToggleList::getSelection(int i) { return _selmap.at(i); }

std::string ToggleList::render() {
  internal_update();

  std::string outbuff;
  std::string icon;

  for (int i = _start_line; i < std::min(_elements.size(), static_cast<size_t>(_start_line + _visible_lines)); i++) {
    Text element(_elements.at(i), Styles::none, _w - 2, 1);

    if (getSelection(i)) {
      icon = unicode::TICK;
    } else {
      icon = unicode::DOT;
    }

    if (i + _start_line == _cursor) {
      outbuff += renderer.render(icon + " " + element.render(), _focus_style);
    } else {
      outbuff += renderer.render(icon + " " + element.render(), _blur_style);
    }

    outbuff += curs_left(_w); // Text is guaranteed to be w-2 width
    outbuff += curs_down(_line_spacing + 1);
  }

  return outbuff;
}

void ToggleList::internal_update() {
  _visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (_visible_lines + 1) + (_line_spacing * _visible_lines);

    if (space_used > _h) {
      break;
    }
    _visible_lines++;
  }
}

} // namespace termui
