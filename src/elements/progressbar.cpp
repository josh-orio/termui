#include "elements.hpp"

namespace termui {

ProgressBar::ProgressBar(int width, float completion, std::string fg_col, std::string bg_col)
    : width(width), completion(completion), fg_col(fg_col), bg_col(bg_col){};

std::string ProgressBar::render() {
  std::string outbuff;

  outbuff += fg_col;
  for (int i = 0; i < completion * width; i++) {
    outbuff += "█";
  }

  outbuff += bg_col;
  for (int i = completion * width; i < width; i++) {
    outbuff += "█";
  }

  outbuff += format::NONE;

  return outbuff;
}

} // namespace termui
