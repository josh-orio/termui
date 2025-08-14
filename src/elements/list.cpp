#include "elements.hpp"

namespace termui {

List::List() = default;
List::List(const std::vector<item::ListItem> &rE, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::ListItem>>(rE)), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {}
List::List(std::vector<item::ListItem> &&lE, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::ListItem>>(std::move(lE))), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {}
List::List(std::shared_ptr<std::vector<item::ListItem>> sharedE, int w, int h, int ls, int col)
    : elements(std::move(sharedE)), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {}

List::List(std::vector<std::string> &lStrs, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::ListItem>>()), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  (*elements).reserve(lStrs.size()); // pre‑allocate

  for (const auto &str : lStrs) {
    (*elements).emplace_back(str);
  }
}
List::List(std::vector<std::string> &&rStrs, int w, int h, int ls, int col) : w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  (*elements).reserve(rStrs.size());

  for (auto &&str : rStrs) {
    (*elements).emplace_back(std::move(str));
  }
}
List::List(std::shared_ptr<std::vector<std::string>> sharedStrs, int w, int h, int ls, int col)
    : w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  if (sharedStrs) {
    (*elements).reserve((*sharedStrs).size());

    for (const auto &str : *sharedStrs) {
      (*elements).emplace_back(str);
    }
  }
}

std::shared_ptr<std::vector<item::ListItem>> List::share() const { return elements; }

const std::vector<item::ListItem> &List::getElements() const { return *elements; }
std::vector<item::ListItem> &List::getElements() { return *elements; }

std::string List::render() {
  internal_update();

  std::string outbuff;

  std::vector<item::ListItem> subset((*elements).begin() + start_line, (*elements).begin() + std::min((int)(*elements).size(), start_line + visible_lines));

  for (int i = 0; i < subset.size(); i++) {
    std::string element(subset.at(i).getText());
    if (i == cursor) {
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
  cursor += (cursor < (*elements).size() - 1) ? 1 : 0;
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

  if (cursor < start_line || start_line + visible_lines < cursor) {
    start_line = cursor;
  }
}

} // namespace termui
