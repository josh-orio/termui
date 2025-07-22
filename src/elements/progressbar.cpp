#include "elements.hpp"

namespace termui {

ProgressBar::ProgressBar(int width, float completion, int fg_col, int bg_col)
    : width(width), completion(completion), fg_col(fg_col), bg_col(bg_col){};

std::string ProgressBar::render() {
  std::string outbuff;

  std::string tmp;
  for (int i = 0; i < completion * width; i++) {
    tmp += symbol::BLOCK;
  }
  outbuff += fg_apply(tmp, fg_col);

  tmp = "";
  for (int i = completion * width; i < width; i++) {
    tmp += symbol::BLOCK;
  }
  outbuff += fg_apply(tmp, bg_col);

  return outbuff;
}

} // namespace termui
