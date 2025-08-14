#include "elements.hpp"

namespace termui {

Input::Input() = default;
Input::Input(const std::string &rVal, const std::string &rPlh, int w, int valCol, int plhCol)
    : val(std::make_shared<std::string>(rVal)), plh(std::make_shared<std::string>(rPlh)), w(w), active_col(valCol), idle_col(plhCol) {}
Input::Input(const std::string &lVal, std::string &&lPlh, int w, int valCol, int plhCol)
    : val(std::make_shared<std::string>(lVal)), plh(std::make_shared<std::string>(std::move(lPlh))), w(w), active_col(valCol), idle_col(plhCol) {}
Input::Input(std::shared_ptr<std::string> sharedVal, std::shared_ptr<std::string> sharedPlh, int w, int valCol, int plhCol)
    : val(std::move(sharedVal)), plh(std::move(sharedPlh)), w(w), active_col(valCol), idle_col(plhCol) {}
Input::Input(std::shared_ptr<std::string> sharedVal, std::string &&lPlh, int w, int valCol, int plhCol)
    : val(std::move(sharedVal)), plh(std::make_shared<std::string>(std::move(lPlh))), w(w), active_col(valCol), idle_col(plhCol) {}

const std::string &Input::getVal() const { return *val; }
std::string &Input::getVal() { return *val; }

const std::string &Input::getPlh() const { return *plh; }
std::string &Input::getPlh() { return *plh; }

std::string Input::render() {
  std::string outbuff;

  std::string value = (*val).data();
  std::string placeholder = (*plh).data();

  if (!value.empty()) {
    if (value.length() > w) {
      outbuff += unicode::ELLIPSIS + value.substr(value.length() - w + 1, value.length());
    } else {
      outbuff += value + std::string(w - value.length(), ' ');
    }
    outbuff = fg_apply(outbuff, active_col);
  } else {

    if (placeholder.length() > w) {
      outbuff += placeholder.substr(0, w - 1) + unicode::ELLIPSIS;
    } else {
      outbuff += placeholder + std::string(w - placeholder.length(), ' ');
    }
    outbuff = fg_apply(outbuff, idle_col);
  }

  return outbuff;
}

} // namespace termui
