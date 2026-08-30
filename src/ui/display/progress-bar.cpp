#include <termui/ui/ui.hpp>

namespace termui {

ProgressBar::ProgressBar(unsigned int width, float decimal, Color foreground, Color background) : w(width), decimal(decimal), fg(foreground), bg(background){};

void ProgressBar::render() {
  int shaded_w = decimal * w;

  terminal.StyleStack(Style(fg, Color::Inherit()));
  terminal.write(repeat(unicode::FULL_SHADE, shaded_w));

  terminal.StyleStack(Style(bg, Color::Inherit()));
  terminal.write(repeat(unicode::LIGHT_SHADE, w - shaded_w));

  terminal.StylePop();
  terminal.StylePop();
}

} // namespace termui
