#include "elements.hpp"
#include <string>

namespace termui {

Select::Select(std::vector<std::string> elements, int w, int h, int ls)
    : elements(elements), w(w), h(h), line_spacing(ls) {
  cursor = 0;
  start_line = 0;

  active_color = 29;
}

std::string Select::render() {
  internal_update();

  std::string outbuff;
  for (int i = start_line; i < std::min((int)elements.size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      outbuff += fg_apply("> " + elements[i], active_color);
      outbuff += curs_left(2 + elements[i].size());

    } else {
      outbuff += "  " + elements[i];
      outbuff += curs_left(2 + elements[i].size());
    }

    outbuff += curs_down(line_spacing + 1);
  }

  return outbuff;
}

void Select::cursor_up() { // decrement but dont let (cursor < 0)
  internal_update();
  cursor -= (cursor > 0) ? 1 : 0;
  start_line -= (cursor < start_line) ? 1 : 0;
}

void Select::cursor_down() { // increment but dont let (cursor > options.size)
  internal_update();
  cursor += (cursor < elements.size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
}

void Select::internal_update() {
  visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_lines + 1) + (line_spacing * visible_lines);

    if (space_used > h) {
      break;
    }
    visible_lines++;
  }

  if (cursor < start_line || start_line + visible_lines < cursor) {
    start_line = cursor;
  }
}

} // namespace termui
