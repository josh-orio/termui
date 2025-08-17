#include "elements.hpp"

namespace termui {

Input::Input() = default;

Input::Input(const std::string &rVal, int w, int valCol) : val(std::make_shared<std::string>(rVal)), w(w), valCol(valCol) {}
Input::Input(std::shared_ptr<std::string> sharedVal, int w, int valCol) : val(std::move(sharedVal)), w(w), valCol(valCol) {}

Input::Input(const std::string &rVal, const std::string &rPlh, int w, int valCol, int plhCol)
    : val(std::make_shared<std::string>(rVal)), plh(std::make_shared<std::string>(rPlh)), w(w), valCol(valCol), plhCol(plhCol) {}
Input::Input(const std::string &lVal, std::string &&lPlh, int w, int valCol, int plhCol)
    : val(std::make_shared<std::string>(lVal)), plh(std::make_shared<std::string>(std::move(lPlh))), w(w), valCol(valCol), plhCol(plhCol) {}
Input::Input(std::shared_ptr<std::string> sharedVal, std::shared_ptr<std::string> sharedPlh, int w, int valCol, int plhCol)
    : val(std::move(sharedVal)), plh(std::move(sharedPlh)), w(w), valCol(valCol), plhCol(plhCol) {}
Input::Input(std::shared_ptr<std::string> sharedVal, std::string &&lPlh, int w, int valCol, int plhCol)
    : val(std::move(sharedVal)), plh(std::make_shared<std::string>(std::move(lPlh))), w(w), valCol(valCol), plhCol(plhCol) {}

const std::string &Input::getVal() const { return *val; }
std::string &Input::getVal() { return *val; }

const std::string &Input::getPlh() const { return *plh; }
std::string &Input::getPlh() { return *plh; }

std::string Input::render() {
  std::string outbuff;

  std::string value = (*val).data();
  std::string placeholder = plh ? plh->data() : ""; // mitigate nullptr segfault

  if (!value.empty()) {
    if (value.length() > w) {
      outbuff += unicode::ELLIPSIS + value.substr(value.length() - w + 1, value.length());
    } else {
      outbuff += value + std::string(w - value.length(), ' ');
    }
    outbuff = fg_apply(outbuff, valCol);
  } else if (plh) {
      if (placeholder.length() > w) {
        outbuff += placeholder.substr(0, w - 1) + unicode::ELLIPSIS;
      } else {
        outbuff += placeholder + std::string(w - placeholder.length(), ' ');
      }
      outbuff = fg_apply(outbuff, plhCol);
    
  } else {
    return std::string(w, ' ');
  }

  return outbuff;
}

} // namespace termui
