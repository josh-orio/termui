#include <termui/ui/ui.hpp>

namespace termui {

List::List(const termui::strings &strs, Style focus_style, Style blur_style)
  : _elements(strs), _focus_style(focus_style), _blur_style(blur_style), _start_line(0), _cursor(0) {}

List &List::focus_style(Style s) {
  _focus_style = s;
  return *this;
}

List &List::blur_style(Style s) {
  _blur_style = s;
  return *this;
}

List &List::width(uint w) {
  _w = w;
  return *this;
}

List &List::height(uint h) {
  _h = h;
  return *this;
}

List &List::line_seperation(uint ls) {
  _line_spacing = ls;
  return *this;
}

void List::cursor_up(uint count) {
  internal_update();
  _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
  _start_line = (_cursor < _start_line) ? _cursor : _start_line;   // don't let cursor go 'above' start line
}

void List::cursor_down(uint count) {
  internal_update();
  _cursor = (_cursor < _elements.size() - count) ? std::min(_elements.size() - 1, static_cast<size_t>(_cursor + count))
                                                 : _elements.size() - 1;                                // increment but dont let (cursor > elements.size)
  _start_line = (_cursor >= _start_line + _visible_lines) ? _cursor - _visible_lines + 1 : _start_line; // don't let cursor go 'below' bottom of visible lines
}

uint List::get_cursor() { return _cursor; }

std::string List::render() {
  internal_update();

  std::string outbuff;

  for (int i = _start_line; i < std::min(_elements.size(), static_cast<size_t>(_start_line + _visible_lines)); i++) {
    Text element(_elements.at(i), Styles::none, _w - 2, 1);

    if (i == _cursor) {
      outbuff += renderer.render("> " + element.render(), _focus_style);

    } else {
      outbuff += renderer.render("  " + element.render(), _blur_style);
    }

    outbuff += curs_left(_w); // Text is guaranteed to be w-2 width
    outbuff += curs_down(_line_spacing + 1);
  }

  return outbuff;
}

void List::internal_update() {
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
