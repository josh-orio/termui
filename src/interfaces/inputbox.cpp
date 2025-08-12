#include "interfaces.hpp"

namespace termui {

InputBox::InputBox() = default;
InputBox::InputBox(std::string &f, std::string &r, std::string &p)
    : field(std::make_shared<std::string>(f)), response(std::make_shared<std::string>(r)), placeholder(std::make_shared<std::string>(p)) {
  cons = Console(false, false, false, true);
};
InputBox::InputBox(std::string &&f, std::string &r, std::string &&p)
    : field(std::make_shared<std::string>(std::move(f))), response(std::make_shared<std::string>(r)), placeholder(std::make_shared<std::string>(std::move(p))) {
  cons = Console(false, false, false, true);
};
InputBox::InputBox(std::shared_ptr<std::string> f, std::shared_ptr<std::string> r, std::shared_ptr<std::string> p) : field(f), response(r), placeholder(p) {
  cons = Console(false, false, false, true);
};
InputBox::InputBox(std::string &&f, std::shared_ptr<std::string> r, std::string &&p)
    : field(std::make_shared<std::string>(std::move(f))), response(r), placeholder(std::make_shared<std::string>(std::move(p))) {
  cons = Console(false, false, false, true);
};

std::shared_ptr<std::string> InputBox::shareField() const { return field; }

const std::string &InputBox::getField() const { return *field; }
std::string &InputBox::getField() { return *field; }

std::shared_ptr<std::string> InputBox::shareResponse() const { return response; }

const std::string &InputBox::getResponse() const { return *response; }
std::string &InputBox::getResponse() { return *response; }

void InputBox::show() {
  cons.show(); // configure terminal

  do {
    display();
  } while (process_input());

  cons.close(); // reset terminal
}

void InputBox::display() {
  update_size();

  Box b(w, h, clr::LIGHTGREY);

  cons.print((cons.height - h) / 2, (cons.width - w) / 2, b.render());

  cons.curs_up(h - 2);
  cons.curs_left(w - 2);

  Text f(field, w - 4, 1, clr::MAGENTA, clr::DEFAULT);
  Input r((*response), (*placeholder), w - 4, clr::DEFAULT, clr::DARKGREY);

  //   if ((*response).empty()) {
  //     r.getText() = (*placeholder).data();
  //     r.fg_col=clr::DARKGREY;
  //   } else {
  //     r.getText() = (*response).data();
  //   }

  cons.print(bt(f.render()));
  cons.curs_down(2);
  cons.curs_left(f.w);

  cons.print(r.render());

  cons.print(cons.height, 2, faint_text("[ESC] close"));
  cons.flush();
}

bool InputBox::process_input() {
  std::string ec = cons.poll_input(); // read in a control

  if (ec == key::ESC) { // escape to close
    return false;

  } else if (ec == key::DEL) { // remove last char
                               // DEL char clears last char in string
    if ((*response).size() > 0) {
      (*response) = std::string((*response).begin(), (*response).end() - 1);
    }
    return true;

  } else if ((std::string{32} <= ec) && (ec <= std::string{126})) { // add char
                                                                    // accept ascii characters in range 32 - 126
                                                                    // input sanitization is done in classes, not here
    (*response) += ec;
    return true;

  } else {
    return true;
  }
};

void InputBox::update_size() {
  cons.update_size();

  w = 0.25 * cons.width;
  h = 5;
}

} // namespace termui
