#include "interfaces.hpp"

namespace termui {
InputPage::InputPage() = default;
InputPage::InputPage(std::string &t, std::vector<std::string> &f, std::vector<std::string> &r, int ls) // existing t + existing vectors of strings
    : title(std::make_shared<std::string>(t)), fields(nullptr), responses(nullptr), line_seperation(ls) {
  cons = Console(false, false, false, true);

  fields = std::make_shared<std::vector<std::string>>(f);
  responses = std::make_shared<std::vector<std::string>>(r);
};
InputPage::InputPage(std::string &t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r, int ls) // shared t + .share()
    : title(std::make_shared<std::string>(t)), fields(f), responses(r), line_seperation(ls) {
  cons = Console(false, false, false, true);
};

InputPage::InputPage(std::string &&t, std::vector<std::string> &f, std::vector<std::string> &r, int ls) // rvalue t + existing vectors of strings
    : title(std::make_shared<std::string>(std::move(t))), fields(nullptr), responses(nullptr), line_seperation(ls) {
  cons = Console(false, false, false, true);

  fields = std::make_shared<std::vector<std::string>>(f);
  responses = std::make_shared<std::vector<std::string>>(r);
};
InputPage::InputPage(std::string &&t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r, int ls) // rvalue t + .share()
    : title(std::make_shared<std::string>(std::move(t))), fields(f), responses(r), line_seperation(ls) {
  cons = Console(false, false, false, true);
};

InputPage::InputPage(std::shared_ptr<std::string> t, std::vector<std::string> &f, std::vector<std::string> &r, int ls) // shared t + existing vectors of strings
    : title(t), fields(nullptr), responses(nullptr), line_seperation(ls) {
  cons = Console(false, false, false, true);

  fields = std::make_shared<std::vector<std::string>>(f);
  responses = std::make_shared<std::vector<std::string>>(r);
};
InputPage::InputPage(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r,
                     int ls) // all .share()
    : title(t), fields(f), responses(r), line_seperation(ls) {
  cons = Console(false, false, false, true);
};

std::shared_ptr<std::vector<std::string>> InputPage::shareFields() const { return fields; }

const std::vector<std::string> &InputPage::getFields() const { return *fields; }
std::vector<std::string> &InputPage::getFields() { return *fields; }

std::shared_ptr<std::vector<std::string>> InputPage::shareResponses() const { return responses; }

const std::vector<std::string> &InputPage::getResponses() const { return *responses; }
std::vector<std::string> &InputPage::getResponses() { return *responses; }

int InputPage::show() {
  cons.show(); // configure terminal

  selected = false;

  int sig;
  do {
    display();
    sig = process_input();
  } while (sig != -1);

  cons.close(); // reset terminal

  if (sig == -1) { // return codes:
    return -1;     // -1 : normal exit
  } else {         // >= 0 : selection at cursor position
    return cursor;
  }
}

void InputPage::display() {
  update_size();

  cons.print(2, 2, (*title));

  // replace with input objects (?)

  int space_used = 0;
  for (int i = start_line; i < std::min((int)(*fields).size(), start_line + visible_lines); i++) {
    if (i == cursor) {
      if (selected) {
        cons.print(space_used + 4, 2, bt("> " + (*fields)[i]) + ": " + rv((*responses)[i] + " "));
      } else {
        cons.print(space_used + 4, 2, bt("> " + (*fields)[i]) + ": " + (*responses)[i]);
      }
    } else {
      cons.print(space_used + 4, 4, (*fields)[i] + ": " + (*responses)[i]);
    }
    space_used++;

    for (int ii = 0; ii < line_seperation; ii++) {
      if (space_used < cons.height - overhead) {
        space_used++;
      }
    }
  }

  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select/deselect"));
  cons.flush();
}
int InputPage::process_input() {
  // return values:
  // -1: close window
  // 0: normal/proceed
  // 1: element selected

  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ENTER) { // select/deselect
    if (selected) {
      selected = false;
      return 0;
    } else {
      selected = true;
      return 1;
    }

  } else if (ec == key::ESC) { // escape to close
    if (!selected) {           // dont close if field selected
      return -1;
    }
    return 0;

  } else if (ec == key::U_ARROW) {
    if (!selected) {
      // up arrow moves cursor up
      // decrement but dont let (cursor < 0)
      cursor -= (cursor > 0) ? 1 : 0;
      start_line -= (cursor < start_line) ? 1 : 0;
    }
    return 0;

  } else if (ec == key::D_ARROW) {
    if (!selected) {
      // down arrow moves cursor down
      // increment but dont let (cursor > fields.size)
      cursor += (cursor < (*fields).size() - 1) ? 1 : 0;
      start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
    }
    return 0;

  } else if (ec == key::DEL) { // remove last char
    // DEL char clears last char in string
    if (selected) {
      if ((*responses)[cursor].size() > 0) {
        (*responses)[cursor] = std::string((*responses)[cursor].begin(), (*responses)[cursor].end() - 1);
      }
    }
    return 0;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // add char
    // accept ascii characters in range 32 - 126
    // input sanitization is done in classes, not here
    if (selected) {
      (*responses)[cursor] += ec;
    }
    return 0;

  } else {
    return 0;
  }
};

void InputPage::update_size() {
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
