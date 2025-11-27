#include <termui/elements.hpp>

namespace termui {

Input::Input() = default;
Input::Input(const termui::string &val, const termui::string &plh, int w, int valCol, int plhCol) : val(val), plh(plh), w(w), valCol(valCol), plhCol(plhCol) {}
Input::Input(const termui::string &val, const std::string &plh, int w, int valCol, int plhCol) : val(val), plh(plh), w(w), valCol(valCol), plhCol(plhCol) {}
Input::Input(const termui::string &val, int w, int valCol) : val(val), w(w), valCol(valCol) {}
Input::Input(std::shared_ptr<std::string> val, std::shared_ptr<std::string> plh, int w, int valCol, int plhCol)
    : val(val), plh(plh), w(w), valCol(valCol), plhCol(plhCol) {}
Input::Input(std::shared_ptr<std::string> val, const std::string &plh, int w, int valCol, int plhCol)
    : val(val), plh(plh), w(w), valCol(valCol), plhCol(plhCol) {}
Input::Input(std::shared_ptr<std::string> val, int w, int valCol) : val(val), w(w), valCol(valCol) {}

const std::string &Input::getVal() const { return val.text(); }
std::string &Input::getVal() { return val.text(); }

const std::string &Input::getPlh() const { return plh.text(); }
std::string &Input::getPlh() { return plh.text(); }

std::string Input::render() {
  std::string outbuff;

  std::string value = val.text();
  std::string placeholder = plh.text();

  if (!value.empty()) {
    if (value.length() > w) {
      outbuff += unicode::ELLIPSIS + value.substr(value.length() - w + 1, value.length());
    } else {
      outbuff += value + std::string(w - value.length(), ' ');
    }
    outbuff = fg_apply(outbuff, valCol);
  } else if (!placeholder.empty()) {
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
