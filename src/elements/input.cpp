#include "elements.hpp"

namespace termui {

Input::Input(std::string field, std::string &value, std::string placeholder, std::string ftc, bool vertical)
    : field(field), value(value), placeholder(placeholder), ftc(ftc), vertical(vertical) {
  max_w = -1; // max size_t
}

std::string Input::render() {
  std::string outbuff;

  if (vertical) {
    outbuff += fg_apply(field, ftc);
    outbuff += curs_left(field.size()) + curs_down(1);
  } else {
    outbuff += fg_apply(field, ftc) + ": ";
  }

  if (value.empty()) {
    outbuff += fg_color(242); // place holder color doesnt need to be accessed
    if (placeholder.size() > max_w) {
      outbuff += std::string(placeholder.begin(), placeholder.begin() + max_w) + symbol::ELLIPSIS;
      outbuff += curs_left(max_w + 1); // so cursor can blink over start of placeholder

    } else {
      outbuff += placeholder;
      outbuff += curs_left(placeholder.size());
    }

    outbuff += fg::DEFAULT;

  } else {
    if (value.size() > max_w) {
      outbuff += symbol::ELLIPSIS + std::string(value.end() - max_w, value.end());

    } else {
      outbuff += value;
    }
  }

  if (active) {
    outbuff += term::SHOW_CURSOR; // this will still interfere, change soon
  }

  return outbuff;
}

} // namespace termui
