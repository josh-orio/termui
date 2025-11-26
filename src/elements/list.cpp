#include <termui/elements.hpp>

namespace termui {

List::List(const termui::strings &strs, int w, int h, int ls, int col)
    : elements(strs), start_line(0), w(w), h(h), cursor(0), line_spacing(ls), active_color(col) {}

std::string List::render() {
  internal_update();

  std::string outbuff;

  std::string element;
  for (int i = start_line; i < std::min((int)elements.size(), start_line + visible_lines); i++) {
    element = elements.getItem(i);
    if (i + start_line == cursor) {
      outbuff += fg_apply("> " + element, active_color);
      outbuff += curs_left(2 + element.size());

    } else {
      outbuff += "  " + element;
      outbuff += curs_left(2 + element.size());
    }

    outbuff += curs_down(line_spacing + 1);
  }

  return outbuff;
}

void List::cursor_up() { // decrement but dont let (cursor < 0)
  internal_update();
  cursor -= (cursor > 0) ? 1 : 0;
  start_line -= (cursor < start_line) ? 1 : 0;
}

void List::cursor_down() { // increment but dont let (cursor > elements.size)
  internal_update();
  cursor += (cursor < elements.size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
}

void List::internal_update() {
  visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_lines + 1) + (line_spacing * visible_lines);

    if (space_used > h) {
      break;
    }
    visible_lines++;
  }
}

} // namespace termui
