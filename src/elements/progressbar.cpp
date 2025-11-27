#include <termui/elements.hpp>

namespace termui {

ProgressBar::ProgressBar(int width, float completion, int fg_col, int bg_col) : width(width), completion(completion), fg_col(fg_col), bg_col(bg_col){};

std::string ProgressBar::render() {
  std::string outbuff;

  outbuff += bg_apply(std::string(completion * width, ' '), fg_col);
  outbuff += bg_apply(std::string(width - (completion * width), ' '), bg_col);

  return outbuff;
}

} // namespace termui
