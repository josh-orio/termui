#include "interfaces.hpp"

namespace termui {

InputPage::InputPage(const termui::string &title, const termui::strings &fields, const termui::strings &responses, int ls, int col)
    : title(title), fields(fields), responses(responses), cons(), line_seperation(ls), col(col), voh(5), cursor(0), start_line(0) {}
InputPage::InputPage(const std::string &title, const termui::strings &fields, const termui::strings &responses, int ls, int col)
    : title(title), fields(fields), responses(responses), cons(), line_seperation(ls), col(col), voh(5), cursor(0), start_line(0) {}
InputPage::InputPage(const std::string &title, const std::vector<std::string> &fields, const termui::strings &responses, int ls, int col)
    : title(title), fields(fields), responses(responses), cons(), line_seperation(ls), col(col), voh(5), cursor(0), start_line(0) {}

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
  } while (sig != -1); // this gotta be wrong

  cons.close(); // reset terminal

  if (sig == -1) { // return codes:
    return -1;     // -1 : normal exit
  } else {         // >= 0 : selection at cursor position
    return cursor;
  }
}

void InputPage::display() {
  update_size();

  cons.print(2, 2, bt(title.text()));

  cons.print(4, 2, ""); // repositioning to start printing input lines
  std::string fld;      //, rsp;
  Input rsp;
  for (int i = start_line; i < std::min((int)fields.size(), start_line + visible_lines); i++) {
    fld = fields.getItem(i);
    rsp = Input(responses.shareItem(i), cons.width - /*some horizonstal overhead*/ 7 - fld.length());

    if (i + start_line == cursor) {
      cons.print(fg_apply(bt("> " + fld) + ": " + rsp.render(), col));
      cons.print((curs_down(line_seperation + 1) + curs_left(4 + fld.length() + rsp.w)));
    } else {
      cons.print("  " + fld + ": " + rsp.render());
      cons.print((curs_down(line_seperation + 1) + curs_left(4 + fld.length() + rsp.w)));
    }
  }

  cons.print(cons.height, 2, faint_text("[ESC] close  [↑/↓] scroll [↵] select/deselect"));

  // move the cursor into position and adjust cursor toggle as needed
  if (selected) {
    cons.print(4 + ((cursor - start_line) * (line_seperation + 1)), 6 + fields.getItem(cursor).length() + responses.getItem(cursor).length(),
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
      cursor += (cursor < fields.size() - 1) ? 1 : 0;
      start_line += (cursor >= start_line + visible_lines) ? 1 : 0;
    }
    return 0;

  } else if (ec == key::DEL) { // remove last char
    if (selected) {
      if (responses.getItem(cursor).size() > 0) {
        responses.getItem(cursor) = std::string(responses.getItem(cursor).begin(), responses.getItem(cursor).end() - 1);
      }
    }
    return 0;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // accept basically all chars
    if (selected) {
      responses.getItem(cursor) += ec;
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

    if (space_used > (cons.height - voh)) {
      break;
    }
    visible_lines++;
  }
}

} // namespace termui
