#include "interfaces.hpp"
#include "util.hpp"

namespace termui {

InfoBox::InfoBox(const termui::string &t, const termui::string &c) : title(t), content(c), cons(), w(15), h(5) {}
InfoBox::InfoBox(const std::string &t, const std::string &c) : title(t), content(c), cons(), w(15), h(5) {}

void InfoBox::show() {
  cons.show(); // configure terminal

  do {
    display();
    process_input();
  } while (state == state::CONTINUE);

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

void InfoBox::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ESC) { // ESC closes info box
    state = state::EXIT;

  } else if (ec == key::ENTER) {
    state = state::EXIT;

  } else {
    state = state::CONTINUE;
  }
}

void InfoBox::update_size() {
  cons.update_size();

  w = 0.25 * cons.width;
  h = 10;
}

} // namespace termui
