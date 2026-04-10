#include <termui/ui/ui.hpp>

namespace termui {

ProgressBar::ProgressBar(uint width, float decimal, Color foreground, Color background) : w(width), decimal(decimal), fg(foreground), bg(background) {};

std::string ProgressBar::render() {
  std::string outbuff;

  int comp_w = decimal * w;

  outbuff += renderer.render(repeat(unicode::FULL_BLOCK, comp_w), Style(fg, std::nullopt));
  outbuff += renderer.render(repeat(unicode::LIGHT_SHADE, w - comp_w), Style(bg, std::nullopt));

  return outbuff;
}

} // namespace termui
