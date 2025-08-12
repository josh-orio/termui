#include "interfaces.hpp"

namespace termui {

FancyMenu::FancyMenu() = default;
FancyMenu::FancyMenu(std::string &t, std::vector<std::string> &e, std::vector<std::string> &d, int ls)
    : title(std::make_shared<std::string>(t)), list(nullptr), line_seperation(ls) {
  cons = Console(false, false, false, true);
  list = std::make_shared<FancyList>(e, d, cons.width - 4, cons.height - 4, ls);
}
FancyMenu::FancyMenu(std::string &t, std::vector<item::FancyListItem> &e, int ls)
    : title(std::make_shared<std::string>(t)), list(nullptr), line_seperation(ls) {
  cons = Console(false, false, false, true);
  list = std::make_shared<FancyList>(e, cons.width - 4, cons.height - 4, ls);
}
FancyMenu::FancyMenu(std::string &&t, std::vector<std::string> &&e, std::vector<std::string> &&d, int ls)
    : title(std::make_shared<std::string>(std::move(t))), list(nullptr), line_seperation(ls) {
  cons = Console(false, false, false, true);
  list = std::make_shared<FancyList>(e, d, cons.width - 4, cons.height - 4, ls);
}
FancyMenu::FancyMenu(std::string &&t, std::vector<item::FancyListItem> &&e, int ls)
    : title(std::make_shared<std::string>(std::move(t))), list(nullptr), line_seperation(ls) {
  cons = Console(false, false, false, true);
  list = std::make_shared<FancyList>(e, cons.width - 4, cons.height - 4, ls);
}
FancyMenu::FancyMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> e, std::shared_ptr<std::vector<std::string>> d, int ls)
    : title(std::move(t)), list(), line_seperation(ls) {
  cons = Console(false, false, false, true);

  (*list).getElements().reserve((*e).size());

  auto it1 = (*e).begin();
  auto it2 = (*d).begin();

  for (; it1 != (*e).end() && it2 != (*d).end(); ++it1, ++it2) {
    (*list).getElements().emplace_back(*it1, *it2);
  }
}
FancyMenu::FancyMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<item::FancyListItem>> e, int ls)
    : title(std::move(t)), list(std::make_shared<FancyList>()), line_seperation(ls) {
  cons = Console(false, false, false, true);

  (*list).getElements().reserve((*e).size());
  for (const auto &elem : *e) {
    (*list).getElements().emplace_back(elem);
  }
}

int FancyMenu::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close();  // reset terminal
  return cursor; // returns selected option
}

void FancyMenu::display() {
  update_size();

  cons.print(2, 2, (*title));
  cons.print(4, 2, (*list).render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select"));
  cons.flush();
}
bool FancyMenu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    (*list).cursor_up();
    return true;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    (*list).cursor_down();
    return true;

  } else if (ec == key::ENTER) { // enter selects the option
    return false;

  } else if (ec == key::ESC) { // esc closes the interface
    return false;

  } else {
    return true;
  }
};

void FancyMenu::update_size() { cons.update_size(); }

} // namespace termui
