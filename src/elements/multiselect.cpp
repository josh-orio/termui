#include "elements.hpp"

namespace termui {

MultiSelect::MultiSelect(std::vector<std::string> elements, int w, int h, int ls)
    : elements(elements), w(w), h(h), line_spacing(ls) {
  cursor = 0;
  start_line = 0;

  selection_map = std::vector(elements.size(), false);
}

std::string MultiSelect::render() {
  internal_update();
  std::string outbuff;

  for (int i = start_line; i < std::min((int)elements.size(), start_line + visible_lines); i++) {

    std::string tmp = elements[i]; // do trimming if required

    outbuff += std::format("{}{} {} {}{}", (i == cursor) ? fg_color(29) : "", (i == cursor) ? ">" : " ",
                           (selection_map[i]) ? symbol::TICK : symbol::DOT, tmp, (i == cursor) ? format::NONE : "");

    outbuff += curs_left(2 + 2 + elements[i].size()); // + curs_down(1);

    outbuff += curs_down(line_spacing + 1);
  }

  return outbuff;
}

void MultiSelect::cursor_up() { // decrement but dont let (cursor < 0)
  cursor -= (cursor > 0) ? 1 : 0;
  start_line -= (cursor < start_line) ? 1 : 0;
}

void MultiSelect::cursor_down() { // increment but dont let (cursor > options.size)
  cursor += (cursor < elements.size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + h) ? 1 : 0;
}

void MultiSelect::select() { selection_map[cursor] = !selection_map[cursor]; }

void MultiSelect::internal_update() {
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
