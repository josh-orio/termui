#include "elements.hpp"

namespace termui {
SelectList::SelectList() = default;
SelectList::SelectList(const std::vector<item::MultiListItem> &e, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::MultiListItem>>(e)), w(w), h(h), line_spacing(ls), active_color(col) {}
SelectList::SelectList(std::vector<item::MultiListItem> &&e, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::MultiListItem>>(std::move(e))), w(w), h(h), line_spacing(ls), active_color(col) {}
SelectList::SelectList(std::shared_ptr<std::vector<item::MultiListItem>> shared, int w, int h, int ls, int col)
    : elements(std::move(shared)), w(w), h(h), line_spacing(ls), active_color(col) {}

SelectList::SelectList(std::vector<std::string> &lStrs, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::MultiListItem>>()), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  (*elements).reserve(lStrs.size());

  for (const auto &str : lStrs) {
    (*elements).emplace_back(str);
  }
}
SelectList::SelectList(std::vector<std::string> &&rStrs, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::MultiListItem>>()), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  (*elements).reserve(rStrs.size()); // avoid re‑allocations

  for (auto &&str : rStrs) {
    (*elements).emplace_back(std::move(str));
  }
}
SelectList::SelectList(std::shared_ptr<std::vector<std::string>> sharedStrs, int w, int h, int ls, int col)
    : elements(std::make_shared<std::vector<item::MultiListItem>>()), w(w), h(h), line_spacing(ls), active_color(col), cursor(0), start_line(0) {
  if (sharedStrs) {
    (*elements).reserve((*sharedStrs).size());

    for (const auto &str : *sharedStrs) {
      (*elements).emplace_back(str);
    }
  }
}

std::shared_ptr<std::vector<item::MultiListItem>> SelectList::share() const { return elements; }

const std::vector<item::MultiListItem> &SelectList::getElements() const { return *elements; }
std::vector<item::MultiListItem> &SelectList::getElements() { return *elements; }

std::string SelectList::render() {
  internal_update();

  std::string outbuff;

  std::vector<item::MultiListItem> subset((*elements).begin() + start_line,
                                          (*elements).begin() + std::min((int)(*elements).size(), start_line + visible_lines));

  for (int i = 0; i < subset.size(); i++) {
    std::string text(subset.at(i).getText());

    std::string tmp = text; // do trimming if required
    int rtl = tmp.length();

    if (subset.at(i).isSelected()) {
      tmp = unicode::TICK + " " + tmp;
    } else {
      tmp = unicode::DOT + " " + tmp;
    }

    if (i + start_line == cursor) {
      tmp = fg_apply(tmp, active_color);
    }

    outbuff += tmp;
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
  cursor += (cursor < (*elements).size() - 1) ? 1 : 0;
  start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
}

void SelectList::select() { (*elements)[cursor].flip(); }

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
