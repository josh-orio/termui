#include <termui/interfaces.hpp>

namespace termui {

InputBox::InputBox(const termui::string &field, const termui::string &response, const termui::string &placeholder)
    : field(field), response(response), placeholder(placeholder), cons() {}
InputBox::InputBox(const std::string &field, const termui::string &response, const std::string &placeholder)
    : field(field), response(response), placeholder(placeholder), cons() {}

std::shared_ptr<std::string> InputBox::shareField() const { return field.share(); }

const std::string &InputBox::getField() const { return field.text(); }
std::string &InputBox::getField() { return field.text(); }

std::shared_ptr<std::string> InputBox::shareResponse() const { return response.share(); }

const std::string &InputBox::getResponse() const { return response.text(); }
std::string &InputBox::getResponse() { return response.text(); }

void InputBox::show() {
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
}

void InputBox::display() {
  update_size();

  cons.print(cons.height, 2, faint_text("[ESC] close")); // print ctl strip to avoid interfering with the cursor

  Box b(w, h, clr::LIGHTGREY);
  cons.print((cons.height - h) / 2, (cons.width - w) / 2, b.render());
  cons.print(curs_up(h - 2) + curs_left(w - 2));

  Text f(field, w - 4, 1, clr::MAGENTA, clr::DEFAULT);
  Input r(response, placeholder, w - 4, 1, clr::DEFAULT, clr::DARKGREY);

  cons.print(bold_text(f.render()));
  cons.print(curs_down(2) + curs_left(f.w));

  cons.print(r.render());
  if (response.text().empty()) {
    cons.print(curs_left(r.w));
  } else {
    auto left_adjust = (response.text().length() < r.w) ? r.w - response.text().length() : 0;
    cons.print(curs_left(left_adjust));
  }
  cons.ct.on(); // enable blinking cursor at start of plh or end of val

  cons.flush();
}

void InputBox::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ESC) { // escape to close
    state = state::EXIT;

  } else if (ec == key::DEL) { // remove last char
    if (response.text().size()) {
      response.text() = std::string(response.text().begin(), response.text().end() - 1);
    }
    reprint = true;
    state = state::CONTINUE;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // accept basically all chars
    response.text() += ec;
    reprint = true;
    state = state::CONTINUE;

  } else {
    state = state::CONTINUE;
  }
};

void InputBox::update_size() {
  cons.update_size();

  w = 0.25 * cons.width;
  h = 5;
}

} // namespace termui
