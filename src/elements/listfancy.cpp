#include <termui/elements.hpp>

namespace termui {

FancyList::FancyList(const termui::strings &tstrs, const termui::strings &dstrs, int w, int h, int ls, int col)
    : text(tstrs), desc(dstrs), start_line(0), w(w), h(h), cursor(0), line_spacing(ls), active_color(col) {}

const std::string &FancyList::getText(int i) const { return text.getItem(i); }
std::string &FancyList::getText(int i) { return text.getItem(i); }

const std::string &FancyList::getDesc(int i) const { return desc.getItem(i); }
std::string &FancyList::getDesc(int i) { return desc.getItem(i); }

std::string FancyList::render() {
  internal_update();

  std::string outbuff;

  std::string t, d;
  for (int i = start_line; i < std::min((int)text.size(), start_line + visible_rows); i++) {
    t = text.getItem(i);
    d = desc.getItem(i);

    if (i + start_line == cursor) {
      outbuff += fg_apply(bold_text(unicode::VERTICAL_WIDE + " " + t), active_color);
      outbuff += curs_left(2 + t.size()) + curs_down(1);
      outbuff += fg_apply(bold_text(unicode::VERTICAL_WIDE + " "), active_color) + fg_apply(d, clr::LIGHTGREY);
      outbuff += curs_left(2 + d.size());

    } else {
      outbuff += "  " + bold_text(t);
      outbuff += curs_left(2 + t.size()) + curs_down(1);
      outbuff += "  " + fg_apply(d, clr::LIGHTGREY);
      outbuff += curs_left(2 + d.size());

    } // might want to replace this block

    outbuff += curs_down(line_spacing + 1);
  }

  return outbuff;
}

void FancyList::cursor_up() { // decrement but dont let (cursor < 0)
  internal_update();
  cursor -= (cursor > 0) ? 1 : 0;
  start_line -= (cursor < start_line) ? 1 : 0;
}

void FancyList::cursor_down() { // increment but dont let (cursor > elements.size)
  internal_update();
  cursor += (cursor < (int) text.size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + visible_rows) ? 1 : 0;
}

void FancyList::internal_update() {
  visible_rows = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (2 * (visible_rows + 1)) + (line_spacing * visible_rows); // 2* because cell is 2 lines high

    if (space_used > h) {
      break;
    }
    visible_rows++;
  }
}

} // namespace termui
