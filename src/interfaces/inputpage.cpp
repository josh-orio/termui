#include "interfaces.hpp"
#include <vector>

namespace termui {
InputPage::InputPage() = default;
InputPage::InputPage(std::string &t, std::vector<std::string> &f, std::vector<std::string> &r, int ls, int col) // existing t + existing vectors of strings
    : title(std::make_shared<std::string>(t)), fields(std::make_shared<std::vector<std::string>>(f)), responses(std::make_shared<std::vector<Input>>()), cons(),
      line_seperation(ls), col(col), voh(5) {

  (*responses).reserve(r.size());
  for (const auto &elem : r) {
    (*responses).emplace_back(elem, 1 /* updated in update_size */);
  }
};
InputPage::InputPage(std::string &t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r, int ls,
                     int col) // shared t + .share()
    : title(std::make_shared<std::string>(t)), fields(f), responses(std::make_shared<std::vector<Input>>()), line_seperation(ls), col(col), voh(5) {

  (*responses).reserve((*r).size());
  for (const auto &elem : *r) {
    (*responses).emplace_back(elem, 1 /* updated in update_size */);
  }
};

InputPage::InputPage(std::string &&t, std::vector<std::string> &f, std::vector<std::string> &r, int ls, int col) // rvalue t + existing vectors of strings
    : title(std::make_shared<std::string>(std::move(t))), fields(std::make_shared<std::vector<std::string>>(f)),
      responses(std::make_shared<std::vector<Input>>()), line_seperation(ls), col(col), voh(5) {

  (*responses).reserve(r.size());
  for (const auto &elem : r) {
    (*responses).emplace_back(elem, 1 /* updated in update_size */);
  }
};
InputPage::InputPage(std::string &&t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r, int ls,
                     int col) // rvalue t + .share()
    : title(std::make_shared<std::string>(std::move(t))), fields(f), responses(std::make_shared<std::vector<Input>>()), line_seperation(ls), col(col), voh(5) {

  (*responses).reserve((*r).size());
  for (const auto &elem : *r) {
    (*responses).emplace_back(elem, 1 /* updated in update_size */);
  }
};

InputPage::InputPage(std::shared_ptr<std::string> t, std::vector<std::string> &f, std::vector<std::string> &r, int ls,
                     int col) // shared t + existing vectors of strings
    : title(t), fields(std::make_shared<std::vector<std::string>>(f)), responses(std::make_shared<std::vector<Input>>()), line_seperation(ls), col(col),
      voh(5) {

  (*responses).reserve(r.size());
  for (const auto &elem : r) {
    (*responses).emplace_back(elem, 1 /* updated in update_size */);
  }
};
InputPage::InputPage(std::shared_ptr<std::string> t, std::shared_ptr<std::vector<std::string>> f, std::shared_ptr<std::vector<std::string>> r, int ls,
                     int col) // all .share()
    : title(t), fields(f), responses(std::make_shared<std::vector<Input>>()), line_seperation(ls), col(col), voh(5) {

  (*responses).reserve((*r).size());
  for (const auto &elem : *r) {
    (*responses).emplace_back(elem, 1 /* updated in update_size */);
  }
};

// std::shared_ptr<std::vector<std::string>> InputPage::shareFields() const { return fields; }

// const std::vector<std::string> &InputPage::getFields() const { return *fields; }
// std::vector<std::string> &InputPage::getFields() { return *fields; }

// std::shared_ptr<std::vector<Input>> InputPage::shareResponses() const { return responses; }

// const std::vector<Input> &InputPage::getResponses() const { return *responses; }
// std::vector<Input> &InputPage::getResponses() { return *responses; }

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

  cons.print(2, 2, bt(*title));

  std::vector<std::string> subsetF((*fields).begin() + start_line, (*fields).begin() + std::min((int)(*fields).size(), start_line + visible_lines));
  std::vector<Input> subsetR((*responses).begin() + start_line, (*responses).begin() + std::min((int)(*responses).size(), start_line + visible_lines));

  cons.print(4, 2, ""); // repositioning to start printing input lines
  for (int i = 0; i < subsetF.size(); i++) {
    std::string fld(subsetF.at(i));
    std::string rsp(subsetR.at(i).render());

    if (i + start_line == cursor) {
      cons.print(fg_apply(bt("> " + fld) + ": " + rsp, col));
      // cons.print((curs_down(line_seperation+1) + curs_left(4 + fld.length() + rsp.length())));
      cons.print((curs_down(line_seperation + 1) + curs_left(4 + fld.length() + (*responses).at(i).w)));
    } else {
      cons.print("  " + fld + ": " + rsp);
      cons.print((curs_down(line_seperation + 1) + curs_left(4 + fld.length() + (*responses).at(i).w)));
    }
  }

  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select/deselect"));

  // move the cursor into position and adjust cursor toggle as needed
  if (selected) {
    cons.print(4 + ((cursor - start_line) * (line_seperation + 1)), 6 + (*fields).at(cursor).length() + (*responses).at(cursor).getVal().length(),
               ""); // this looks terrible :/
    cons.ct.on();
  } else {
    cons.ct.off();
  }

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
      // decrement but dont let (cursor < 0)
      cursor -= (cursor > 0) ? 1 : 0;
      start_line -= (cursor < start_line) ? 1 : 0;
    }
    return 0;

  } else if (ec == key::D_ARROW) {
    if (!selected) {
      // increment but dont let (cursor > fields.size)
      cursor += (cursor < (*fields).size() - 1) ? 1 : 0;
      start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
    }
    return 0;

  } else if (ec == key::DEL) { // remove last char
    // DEL char clears last char in string
    if (selected) {
      if ((*responses)[cursor].getVal().size() > 0) {
        (*responses)[cursor].getVal() = std::string((*responses)[cursor].getVal().begin(), (*responses)[cursor].getVal().end() - 1);
      }
    }
    return 0;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // add char
    // accept ascii characters in range 32 - 126
    // input sanitization is done in classes, not here
    if (selected) {
      (*responses)[cursor].getVal() += ec;
    }
    return 0;

  } else {
    return 0;
  }
};

void InputPage::update_size() {
  cons.update_size();

  // update width of each Input element
  auto it1 = (*fields).begin();
  auto it2 = (*responses).begin();
  for (; it1 != (*fields).end() && it2 != (*responses).end(); ++it1, ++it2) {
    (*it2).w = cons.width - /*some horizonstal overhead*/ 7 - (*it1).length();
  }

  visible_lines = 0;
  while (true) {
    // calculates lines used by displaying another row of cells
    int space_used = (visible_lines + 1) + (line_seperation * visible_lines);

    if (space_used > (cons.height - voh)) {
      break;
    }
    visible_lines++;
  }
}

} // namespace termui
