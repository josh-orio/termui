#include "interfaces.hpp"

namespace termui {

Menu::Menu(const std::string &t, const std::vector<std::string> &o, int ls) : title(t), options(o) {
  cons = Console(false, false, false, true);
  line_seperation = ls;
  cursor = 0;
  start_line = 0;
  overhead = 4; // header(3) + footer(1)
}

int Menu::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close(); // reset terminal

  return cursor; // returns selected option
}

void Menu::display() {
  update_size();

  cons.print(2, 2, title);

  int space_used = 0;
  for (int i = start_line; i < std::min((int)options.size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      cons.print(space_used + 4, 2, bt("> " + options[i]));
    } else {
      cons.print(space_used + 4, 4, options[i]);
    }
    space_used++;

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - overhead) {
        space_used++;
      }
    }
  }

  cons.print(cons.height, 2, faint_text("[↵] select  [↑/↓] scroll"));
  cons.flush();
}
bool Menu::process_input() {
  cons.poll_input(); // read in any unread chars

  std::vector<std::string> controls{key::U_ARROW, key::D_ARROW, key::ENTER}; // replacing this will be part of #12
  std::vector<size_t> cep(controls.size());                                  // control - earliest pos for each

  std::transform(controls.begin(), controls.end(), cep.begin(),
                 [this](const std::string &s) { return cons.inbuff.find(s); });

  int min = std::distance(std::begin(cep), std::min_element(std::begin(cep), std::end(cep)));

  if (cep[min] == std::string::npos) {
    return true;
  } // none of the controls are in the buffer

  std::string ec = controls[min]; // earliest control

  cons.inbuff.erase(0, cep[min] + ec.size()); // remove everything upto the end of the control (will erase anything
                                              // unintelligble before the control too)

  // ^ this implementation finds the first control key/sequence in the input buffer. this is done in the odd case that
  // multiple controls may be in the input buffer at once (incredibly unlikely)

  // now interface effects/changes can be handled
  if (ec == key::U_ARROW) { // decrement but dont let (cursor < 0)
    cursor -= (cursor > 0) ? 1 : 0;
    start_line -= (cursor < start_line) ? 1 : 0;
    return true;

  } else if (ec == key::D_ARROW) { // increment but dont let (cursor > options.size)
    cursor += (cursor < options.size() - 1) ? 1 : 0;
    start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
    return true;

  } else if (ec == key::ENTER) { // enter selects the option
    return false;

  } else {
    return true;
  }
};

void Menu::update_size() {
  cons.update_size();

  visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_lines + 1) + (line_seperation * visible_lines);

    if (space_used > (cons.height - overhead)) {
      break;
    }
    visible_lines++;
  }
}

} // namespace termui
