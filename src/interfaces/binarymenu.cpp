#include "interfaces.hpp"

namespace termui {

BinaryMenu::BinaryMenu(const termui::string &title, const termui::string &text, const termui::string &affirmative, const termui::string &negative)
    : title(title), text(text), affirmative(affirmative), negative(negative), cons() {}
BinaryMenu::BinaryMenu(const std::string &title, const std::string &text, const std::string &affirmative, const std::string &negative)
    : title(title), text(text), affirmative(affirmative), negative(negative), cons() {}

bool BinaryMenu::show() {
  cons.show(); // configure terminal

  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    process_input();
  } while (state == state::CONTINUE);

  cons.close(); // reset terminal

  return selection;
}

void BinaryMenu::display() {
  update_size();

  Text t(title, w - 4, 1, clr::PURPLE, clr::DEFAULT);
  Text body(text, w - 4, 2, clr::DEFAULT, clr::DEFAULT);

  Button aff(affirmative, 8, clr::PASTELPINK, clr::DARKGREY);
  Button neg(negative, 8, clr::PASTELPINK, clr::DARKGREY);

  Box b(w, h, clr::DARKGREY);

  cons.print((cons.height - h) / 2, (cons.width - w) / 2, b.render());
  cons.print(curs_up(h - 2) + curs_left(w - 2));

  cons.print(bold_text(t.render()));
  cons.print(curs_down(2) + curs_left(t.w));

  cons.print(body.render());

  if (selection) {
    aff.selected = true;
    neg.selected = false;

  } else /* status == f */ {
    aff.selected = false;
    neg.selected = true;
  }

  cons.print(curs_down(2) + curs_left(aff.w + 1 + neg.w));

  cons.print(aff.render() + " " + neg.render());

  cons.print(cons.height, 2, faint_text(std::format("[{}{}] move [{}] select", unicode::L_ARROW, unicode::R_ARROW, unicode::ENTER)));
  cons.flush();
}

void BinaryMenu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ENTER) {
    state = state::SELECT;

  } else if (ec == key::L_ARROW) {
    if (selection != 1) { // avoid reprint if status already == 1
      selection = true;
      reprint = true;
    }
    state = state::CONTINUE;

  } else if (ec == key::R_ARROW) {
    if (selection != 0) { // avoid reprint if status == 0
      selection = false;
      reprint = true;
    }
    state = state::CONTINUE;

  } else {
    state = state::CONTINUE;
  }
}

void BinaryMenu::update_size() {
  cons.update_size();

  w = 0.25 * cons.width;
  h = 8;
}

} // namespace termui
