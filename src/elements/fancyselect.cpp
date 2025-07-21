#include "elements.hpp"
#include <string>

namespace termui {

FancySelect::FancySelect(std::vector<std::string> elements, std::vector<std::string> desc, int w, int h, int ls)
    : elements(elements), desc(desc), w(w), h(h), line_spacing(ls) {
  cursor = 0;
  start_line = 0;
}

std::string FancySelect::render() {
  internal_update(); //
  std::string outbuff;

  for (int i = start_line; i < std::min((int)elements.size(), start_line + visible_rows); i++) {

    if (i == cursor) {
      outbuff += fg_color(219) + format::BOLD + symbol::VERTICAL + " " + elements[i];
      outbuff += curs_left(2 + elements[i].size()) + curs_down(1);
      outbuff += symbol::VERTICAL + fg::DEFAULT + format::BOLD_OFF + " " + desc[i];
      outbuff += curs_left(2 + desc[i].size());

    } else {
      outbuff += "  " + format::BOLD + elements[i] + format::BOLD_OFF;
      outbuff += curs_left(2 + elements[i].size()) + curs_down(1);
      outbuff += "  " + desc[i];
      outbuff += curs_left(2 + desc[i].size());

    } // might want to replace this block

    outbuff += curs_down(line_spacing + 1);
  }

  return outbuff;
}

void FancySelect::cursor_up() { // decrement but dont let (cursor < 0)
  cursor -= (cursor > 0) ? 1 : 0;
  start_line -= (cursor < start_line) ? 1 : 0;
}

void FancySelect::cursor_down() { // increment but dont let (cursor > elements.size)
  cursor += (cursor < elements.size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + h) ? 1 : 0;
}

void FancySelect::internal_update() {
  visible_rows = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (2 * (visible_rows + 1)) + (line_spacing * visible_rows); // 2* because cell is 2 lines high

    if (space_used > h) {
      break;
    }
    visible_rows++;
  }

  if (cursor < start_line || start_line + visible_rows < cursor) {
    start_line = cursor;
  }
}

} // namespace termui
