#include "elements.hpp"

namespace termui {

FancyList::FancyList() = default;
FancyList::FancyList(std::vector<item::FancyListItem> &e, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::FancyListItem>>(e)), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {}
FancyList::FancyList(std::vector<item::FancyListItem> &&e, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::FancyListItem>>(std::move(e))), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {}
FancyList::FancyList(std::shared_ptr<std::vector<item::FancyListItem>> shared, int w, int h, int ls, int col)
    : elements(std::move(shared)), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {}

FancyList::FancyList(std::vector<std::string> &e, std::vector<std::string> &d, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::FancyListItem>>()), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  (*elements).reserve(e.size());

  auto it1 = e.begin();
  auto it2 = d.begin();

  for (; it1 != e.end() && it2 != d.end(); ++it1, ++it2) {
    (*elements).emplace_back(*it1, *it2);
  }
}
FancyList::FancyList(std::vector<std::string> &&e, std::vector<std::string> &&d, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::FancyListItem>>()), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  (*elements).reserve(e.size());

  auto it1 = e.begin();
  auto it2 = d.begin();

  for (; it1 != e.end() && it2 != d.end(); ++it1, ++it2) {
    (*elements).emplace_back(std::move(*it1), std::move(*it2));
  }
}
FancyList::FancyList(std::shared_ptr<std::vector<std::string>> e, std::shared_ptr<std::vector<std::string>> d, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::FancyListItem>>()), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  if (e) {
    (*elements).reserve((*e).size());

    auto it1 = (*e).begin();
    auto it2 = (*d).begin();

    for (; it1 != (*e).end() && it2 != (*d).end(); ++it1, ++it2) {
      (*elements).emplace_back(*it1, *it2);
    }
  }
}

std::shared_ptr<std::vector<item::FancyListItem>> FancyList::share() const { return elements; }

const std::vector<item::FancyListItem> &FancyList::getElements() const { return *elements; }
std::vector<item::FancyListItem> &FancyList::getElements() { return *elements; }

std::string FancyList::render() {
  internal_update();

  std::string outbuff;

  std::vector<item::FancyListItem> subset((*elements).begin() + start_line, (*elements).begin() + std::min((int)(*elements).size(), start_line + visible_rows));

  for (int i = 0; i < subset.size(); i++) {
    std::string text(subset.at(i).getText());
    std::string desc(subset.at(i).getDesc());

    if (i + start_line == cursor) {
      outbuff += fg_apply(bold_text(unicode::VERTICAL + " " + text), active_color);
      outbuff += curs_left(2 + text.size()) + curs_down(1);
      outbuff += fg_apply(bold_text(unicode::VERTICAL + " "), active_color) + fg_apply(desc, clr::LIGHTGREY);
      outbuff += curs_left(2 + desc.size());

    } else {
      outbuff += "  " + bold_text(text);
      outbuff += curs_left(2 + text.size()) + curs_down(1);
      outbuff += "  " + fg_apply(desc, clr::LIGHTGREY);
      outbuff += curs_left(2 + desc.size());

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
  cursor += (cursor < (*elements).size() - 1) ? 1 : 0;
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
