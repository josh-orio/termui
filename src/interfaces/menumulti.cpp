#include "interfaces.hpp"

namespace termui {

MultiMenu::MultiMenu() = default;
MultiMenu::MultiMenu(std::string &t, std::vector<std::string> &e, int ls) : title(std::make_shared<std::string>(t)), line_seperation(ls), voh(5), hoh(2) {
  cons = Console(false, false, false, true);
  list = std::make_shared<SelectList>(e, cons.width - hoh, cons.height - voh, ls);
}
MultiMenu::MultiMenu(std::string &t, std::vector<item::MultiListItem> &e, int ls)
    : title(std::make_shared<std::string>(t)), line_seperation(ls), voh(5), hoh(2) {
  cons = Console(false, false, false, true);
  list = std::make_shared<SelectList>(e, cons.width - hoh, cons.height - voh, ls);
}
MultiMenu::MultiMenu(std::string &&t, std::vector<std::string> &&e, int ls)
    : title(std::make_shared<std::string>(std::move(t))), line_seperation(ls), voh(5), hoh(2) {
  cons = Console(false, false, false, true);
  list = std::make_shared<SelectList>(e, cons.width - hoh, cons.height - voh, ls);
}
MultiMenu::MultiMenu(std::string &&t, std::vector<item::MultiListItem> &&e, int ls)
    : title(std::make_shared<std::string>(std::move(t))), line_seperation(ls), voh(5), hoh(2) {
  cons = Console(false, false, false, true);
  list = std::make_shared<SelectList>(e, cons.width - hoh, cons.height - voh, ls);
}
MultiMenu::MultiMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> e, int ls)
    : title(std::move(t)), list(std::make_shared<SelectList>()), line_seperation(ls), voh(5), hoh(2) {
  cons = Console(false, false, false, true);

  (*list).getElements().reserve((*e).size());
  for (const auto &str : *e) {
    (*list).getElements().emplace_back(std::move(str));
  }
}
MultiMenu::MultiMenu(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<item::MultiListItem>> e, int ls)
    : title(std::move(t)), list(std::make_shared<SelectList>()), line_seperation(ls), voh(5), hoh(2) {
  cons = Console(false, false, false, true);

  (*list).getElements().reserve((*e).size());
  for (const auto &elem : *e) {
    (*list).getElements().emplace_back(elem);
  }
}

int MultiMenu::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close();  // reset terminal
  return cursor; // returns selected option
}

void MultiMenu::display() {
  update_size();

  cons.print(2, 2, (*title));
  cons.print(4, 2, (*list).render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select"));
  cons.flush();
}
bool MultiMenu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    (*list).cursor_up();
    return true;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    (*list).cursor_down();
    return true;

  } else if (ec == key::ENTER) { // enter toggles the option
    (*list).select();
    return true;

  } else if (ec == key::ESC) { // esc closes the interface
    return false;

  } else {
    return true;
  }

  return true;
};

void MultiMenu::update_size() {
  cons.update_size();

  (*list).h = cons.height - voh;
  (*list).w = cons.width - hoh;
  (*list).line_spacing = line_seperation;
}

} // namespace termui
