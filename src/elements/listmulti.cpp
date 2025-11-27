#include <termui/elements.hpp>

namespace termui {

SelectList::SelectList(const termui::strings &strs, int w, int h, int ls, int col)
    : elements(strs), selmap(strs.size(), false), start_line(0), w(w), h(h), cursor(0), line_spacing(ls), active_color(col) {}

const termui::strings &SelectList::getStrings() const { return elements; }
termui::strings &SelectList::getStrings() { return elements; }

const std::string &SelectList::getElement(int i) const { return elements.getItem(i); }
std::string &SelectList::getElement(int i) { return elements.getItem(i); }

bool SelectList::getSelection(int i) { return selmap[i]; }

std::string SelectList::render() {
  internal_update();

  std::string outbuff;

  std::string text;
  for (int i = start_line; i < std::min((int)elements.size(), start_line + visible_lines); i++) {
    text = elements.getItem(i); // do trimming if required

    int rtl = text.length(); // return length

    if (selmap.at(i)) {
      text = unicode::TICK + " " + text;
    } else {
      text = unicode::DOT + " " + text;
    }

    if (i + start_line == cursor) {
      text = fg_apply(text, active_color);
    }

    outbuff += text;
    outbuff += curs_left(2 + rtl); // accounts for trimming
    outbuff += curs_down(line_spacing + 1);
  }

  return outbuff;
}

void SelectList::cursor_up() { // decrement but dont let (cursor < 0)
  internal_update();
  cursor -= (cursor > 0) ? 1 : 0;
  start_line -= (cursor < start_line) ? 1 : 0;
}

void SelectList::cursor_down() { // increment but dont let (cursor > options.size)
  internal_update();
  cursor += (cursor < elements.size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
}

void SelectList::select() { selmap[cursor] = !selmap[cursor]; }

void SelectList::internal_update() {
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
