#include <termui/ui/ui.hpp>

namespace termui {

FancyList::FancyList(const std::vector<Element> &rows, Style focus_style, Style blur_style)
  : _rows(rows), _focus_style(focus_style), _blur_style(blur_style), _start_line(0), _cursor(0) {}

FancyList &FancyList::focus_style(Style s) {
  _focus_style = s;
  return *this;
}

FancyList &FancyList::blur_style(Style s) {
  _blur_style = s;
  return *this;
}

FancyList &FancyList::width(uint w) {
  _w = w;
  return *this;
}

FancyList &FancyList::height(uint h) {
  _h = h;
  return *this;
}

FancyList &FancyList::line_seperation(uint ls) {
  _line_spacing = ls;
  return *this;
}

void FancyList::cursor_up(uint count) {
  internal_update();
  _cursor = (_cursor > 0) ? std::max(0, int(_cursor - count)) : 0; // decrement but dont let (cursor < 0)
  _start_line = (_cursor < _start_line) ? _cursor : _start_line;   // don't let cursor go 'above' start line
}

void FancyList::cursor_down(uint count) {
  internal_update();
  _cursor = (_cursor < _rows.size() - count) ? std::min(_rows.size() - 1, static_cast<size_t>(_cursor + count))
                                             : _rows.size() - 1;                                      // increment but dont let (cursor > elements.size)
  _start_line = (_cursor >= _start_line + _visible_rows) ? _cursor - _visible_rows + 1 : _start_line; // don't let cursor go 'below' bottom of visible lines
}

uint FancyList::get_cursor() { return _cursor; }

std::string FancyList::render() {
  internal_update();

  std::string outbuff;

  for (int i = _start_line; i < std::min(_rows.size(), static_cast<size_t>(_start_line + _visible_rows)); i++) {
    Text element_top(_rows.at(i).title, Styles::none, _w - 2, 1);
    Text element_bottom(_rows.at(i).description, Styles::faint, _w - 2, 1);

    if (i + _start_line == _cursor) {
      outbuff += renderer.render(unicode::VERTICAL_WIDE + " " + element_top.render(), _focus_style);
      outbuff += curs_down(1) + curs_left(_w);
      outbuff += renderer.render(unicode::VERTICAL_WIDE, _focus_style) + " " + element_bottom.render();

    } else {
      outbuff += "  " + renderer.render(element_top.render(), _blur_style);
      outbuff += curs_down(1) + curs_left(_w);
      outbuff += "  " + element_bottom.render();
    }

    outbuff += curs_left(_w); // Text is guaranteed to be w-2 width
    outbuff += curs_down(_line_spacing + 1);
  }

  return outbuff;
}

void FancyList::internal_update() {
  _visible_rows = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (2 * (_visible_rows + 1)) + (_line_spacing * _visible_rows); // 2* because cell is 2 lines high

    if (space_used > _h) {
      break;
    }
    _visible_rows++;
  }
}

} // namespace termui
