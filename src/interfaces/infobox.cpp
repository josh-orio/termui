#include "interfaces.hpp"

namespace termui {

InfoBox::InfoBox() = default;
InfoBox::InfoBox(const std::string &t, const std::string &c) : title(std::make_shared<std::string>(t)), content(std::make_shared<std::string>(c)), w(15), h(5) {
  cons = Console(false, false, false, true);
}
InfoBox::InfoBox(std::string &&t, std::string &&c)
    : title(std::make_shared<std::string>(std::move(t))), content(std::make_shared<std::string>(std::move(c))), w(15), h(5) {
  cons = Console(false, false, false, true);
}
InfoBox::InfoBox(std::shared_ptr<std::string> sharedT, std::shared_ptr<std::string> sharedC)
    : title(std::move(sharedT)), content(std::move(sharedC)), w(15), h(5) {
  cons = Console(false, false, false, true);
}

void InfoBox::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close(); // reset terminal
}

void InfoBox::display() {
  update_size();

  Text t(title, w - 4, 1, clr::PURPLE, clr::DEFAULT);
  Text body(content, w - 4, 4, clr::DEFAULT, clr::DEFAULT);

  Button aff("Close", 8, clr::PASTELPINK, clr::DARKGREY);
  aff.selected = true;

  Box b(w, h, clr::DARKGREY);

  cons.print((cons.height - h) / 2, (cons.width - w) / 2, b.render());

  cons.curs_up(h - 2);
  cons.curs_left(w - 2);

  cons.print(bt(t.render()));
  cons.curs_down(2);
  cons.curs_left(t.w);

  cons.print(body.render());

  cons.curs_down(2);
  cons.curs_left(aff.w);

  cons.print(aff.render());

  cons.print(cons.height, 2, faint_text(std::format("[{}] close [{}] select", "ESC", unicode::ENTER)));
  cons.flush();
}

bool InfoBox::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ESC) { // ESC closes info box
    return false;

  } else if (ec == key::ENTER) { // enter closes info box
    return false;

  } else {
    return true;
  }
}

void InfoBox::update_size() {
  cons.update_size();

  w = 0.25 * cons.width;
  h = 10;
}

} // namespace termui
