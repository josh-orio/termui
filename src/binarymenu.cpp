#include "interfaces.hpp"

namespace termui {

BinaryMenu::BinaryMenu(std::string title, std::string text, std::string affirmative, std::string negative, int w, int h)
    : title(title), text(text), affirmative(affirmative), negative(negative), w(w), h(h) {
  cons = Console(false, false, false, true);
  status = 1; // default to affirmative
};

bool BinaryMenu::show() {
  cons.show(); // configure terminal

  cons.inbuff.clear();
  reprint = true;

  do {
    if (reprint) {
      display();
      reprint = false;
    }
    cons.await_input();
  } while (process_input());

  cons.close(); // reset terminal

  return (bool)status;
}

bool BinaryMenu::process_input() {
  size_t pos;
  if ((pos = cons.inbuff.find(key::ENTER)) != std::string::npos) {
    return false;

  } else if ((pos = cons.inbuff.find(key::L_ARROW)) != std::string::npos) {
    if (status != 1) { // avoid reprint if status already == 1
      status = 1;
      reprint = true;
    }
    cons.inbuff.erase(0, pos + key::L_ARROW.size());
    return true;

  } else if ((pos = cons.inbuff.find(key::R_ARROW)) != std::string::npos) {
    if (status != 0) { // avoid reprint if status == 0
      status = 0;
      reprint = true;
    }
    cons.inbuff.erase(0, pos + key::R_ARROW.size());
    return true;

  } else {
    return true;
  }
}

void BinaryMenu::display() {
  int pastel_pink = 219;
  int purple = 56;
  int dark_grey = 238; // replace with custom colors

  cons.update_size();
  cons.clear_buffer();

  Text t(title, w - 4, 1, fg_color(purple), format::BOLD);
  Text body(text, w - 4, (text.size() / (w - 4)) + 1, fg::DEFAULT, format::NONE);

  Button aff(8, affirmative, bg_color(pastel_pink), bg_color(dark_grey));
  Button neg(8, negative, bg_color(pastel_pink), bg_color(dark_grey));

  h = 1 + t.h + 1 + body.h + 1 + 1 + 1; // please clean this up, also this overrides height from constructor
  Box b(w, h, fg_color(dark_grey));

  cons.print((cons.height - h) / 2, (cons.width - w) / 2, b.render());

  cons.curs_up(h - 2);
  cons.curs_left(w - 2);

  cons.print(t.render());

  if (!text.empty()) {
    cons.curs_down(2);
    cons.curs_left(t.w);
    cons.print(body.render());
  }

  if (status == 1) {
    aff.selected = true;
    neg.selected = false;

  } else /* status == 0 */ {
    aff.selected = false;
    neg.selected = true;
  }

  cons.curs_down(2);
  cons.curs_left(t.w);

  cons.print(aff.render() + " " + neg.render());

  cons.print(cons.height, 2, faint_text(std::format("[{}] move [{}] select", symbol::HBD, symbol::ENTER)));

  cons.flush();
}

} // namespace termui
