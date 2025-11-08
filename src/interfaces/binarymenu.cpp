#include "interfaces.hpp"

namespace termui {

BinaryMenu::BinaryMenu(const termui::string &title, const termui::string &text, const termui::string &affirmative, const termui::string &negative)
    : cons(), title(title), text(text), affirmative(affirmative), negative(negative) {}
BinaryMenu::BinaryMenu(const std::string &title, const std::string &text, const std::string &affirmative, const std::string &negative)
    : cons(), title(title), text(text), affirmative(affirmative), negative(negative) {}

bool BinaryMenu::show() {
  cons.show(); // configure terminal

  state = state::CONTINUE; // i dont get warned that state is uninit'd? even when -Wuninit is removed from CML
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

  aff_x1 = ((cons.width - w) / 2) + w - (aff.w + neg.w + 3);
  aff_x2 = aff_x1 + aff.w - 1; // x2 should == x1+7 in this case, because that will display as 8 cols
  aff_y = ((cons.height - h) / 2) + h - 2;

  neg_x1 = ((cons.width - w) / 2) + w - (neg.w + 2);
  neg_x2 = neg_x1 + neg.w - 1;
  neg_y = ((cons.height - h) / 2) + h - 2;

  cons.print(aff_y, aff_x1, aff.render());
  cons.print(neg_y, neg_x1, neg.render());

  cons.print(cons.height, 2, faint_text(std::format("[{}{}] move [{}] select", unicode::L_ARROW, unicode::R_ARROW, unicode::ENTER)));
  cons.flush();
}

void BinaryMenu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ENTER) {
    state = state::SELECT;

  } else if (ec == key::L_ARROW) {
    if (!selection) { // avoids reprint if status already == t
      selection = true;
      reprint = true;
    }

  } else if (ec == key::R_ARROW) {
    if (selection) { // avoids reprint if status already == f
      selection = false;
      reprint = true;
    }

  } else if (ec.starts_with('\e')) {
    MouseInteraction mi(ec);

    if (mi.valid) {
      if (mi.match(MouseEventType::LEFT_CLICK, aff_x1, aff_x2, aff_y, aff_y)) {
        selection = true; // ensure pressed button is selected
        state = state::SELECT;
      }

      if (mi.match(MouseEventType::LEFT_CLICK, neg_x1, neg_x2, neg_y, neg_y)) {
        selection = false; // ensure pressed button is selected
        state = state::SELECT;
      }
    }

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
