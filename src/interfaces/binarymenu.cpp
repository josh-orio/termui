#include "interfaces.hpp"

namespace termui {

BinaryMenu::BinaryMenu() = default;
BinaryMenu::BinaryMenu(const std::string &t, const std::string &x, const std::string &aff, const std::string &neg)
    : title(std::make_shared<std::string>(t)), text(std::make_shared<std::string>(x)), affirmative(std::make_shared<std::string>(aff)),
      negative(std::make_shared<std::string>(neg)) {
  cons = Console(false, false, false, true);
}
BinaryMenu::BinaryMenu(std::string &&t, std::string &&x, std::string &&aff, std::string &&neg)
    : title(std::make_shared<std::string>(std::move(t))), text(std::make_shared<std::string>(std::move(x))),
      affirmative(std::make_shared<std::string>(std::move(aff))), negative(std::make_shared<std::string>(std::move(neg))) {
  cons = Console(false, false, false, true);
}
BinaryMenu::BinaryMenu(std::shared_ptr<std::string> sharedT, std::shared_ptr<std::string> sharedX, std::shared_ptr<std::string> sharedAff,
                       std::shared_ptr<std::string> sharedNeg)
    : title(std::move(sharedT)), text(std::move(sharedX)), affirmative(std::move(sharedAff)), negative(std::move(sharedNeg)) {
  cons = Console(false, false, false, true);
}

bool BinaryMenu::show() {
  cons.show(); // configure terminal

  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
  } while (process_input());

  cons.close(); // reset terminal

  return (bool)status;
}

void BinaryMenu::display() {
  update_size();

  Text t(title, w - 4, 1, clr::PURPLE, clr::DEFAULT);
  Text body(text, w - 4, 2, clr::DEFAULT, clr::DEFAULT);

  Button aff(affirmative, 8, clr::PASTELPINK, clr::DARKGREY);
  Button neg(negative, 8, clr::PASTELPINK, clr::DARKGREY);

  Box b(w, h, clr::DARKGREY);

  cons.print((cons.height - h) / 2, (cons.width - w) / 2, b.render());

  cons.curs_up(h - 2);
  cons.curs_left(w - 2);

  cons.print(bt(t.render()));
  cons.curs_down(2);
  cons.curs_left(t.w);

  cons.print(body.render());

  if (status == 1) {
    aff.selected = true;
    neg.selected = false;

  } else /* status == 0 */ {
    aff.selected = false;
    neg.selected = true;
  }

  cons.curs_down(2);
  cons.curs_left(aff.w + 1 + neg.w);

  cons.print(aff.render() + " " + neg.render());

  cons.print(cons.height, 2, faint_text(std::format("[{}] move [{}] select", unicode::HBD, unicode::ENTER)));
  cons.flush();
}

bool BinaryMenu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ENTER) {
    return false;

  } else if (ec == key::L_ARROW) {
    if (status != 1) { // avoid reprint if status already == 1
      status = 1;
      reprint = true;
    }
    return true;

  } else if (ec == key::R_ARROW) {
    if (status != 0) { // avoid reprint if status == 0
      status = 0;
      reprint = true;
    }
    return true;

  } else {
    return true;
  }
}

void BinaryMenu::update_size() {
  cons.update_size();

  w = 0.25 * cons.width;
  h = 8;
}

} // namespace termui
