#include "interfaces.hpp"

namespace termui {

FancyMenu::FancyMenu(const termui::string &title, const termui::strings &texts, const termui::strings &descs, int ls)
    : title(title), text(texts), desc(descs), list(text, desc, 1, 1), line_seperation(ls), cons(), lvo(5), lho(4) {}
FancyMenu::FancyMenu(const std::string &title, const termui::strings &texts, const termui::strings &descs, int ls)
    : title(title), text(texts), desc(descs), list(text, desc, 1, 1), line_seperation(ls), cons(), lvo(5), lho(4) {}
FancyMenu::FancyMenu(const std::string &title, const std::vector<std::string> &texts, const std::vector<std::string> &descs, int ls)
    : title(title), text(texts), desc(descs), list(text, desc, 1, 1), line_seperation(ls), cons(), lvo(5), lho(4) {}

int FancyMenu::show() {
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

  if (state == state::EXIT) {
    return -1;
  } else /* state == SELECT */ {
    return list.cursor; // returns selected option
  }
}

int FancyMenu::cursor() { return list.cursor; }

termui::state FancyMenu::status() { return state; }

void FancyMenu::display() {
  update_size();

  cons.print(2, 2, bt(title.text()));
  cons.print(4, 2, list.render());
  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select"));
  cons.flush();
}

void FancyMenu::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    list.cursor_up();
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    list.cursor_down();
    reprint = true;
    state = state::CONTINUE;

  } else if (ec == key::ENTER) { // enter selects the option
    state = state::SELECT;

  } else if (ec == key::ESC) { // esc closes the interface
    state = state::EXIT;

  } else {
    state = state::CONTINUE;
  }
};

void FancyMenu::update_size() {
  cons.update_size();

  list.h = cons.height - lvo;
  list.w = cons.width - lho;
  list.line_spacing = line_seperation;
}

} // namespace termui
