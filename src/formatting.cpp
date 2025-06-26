#include "formatting.hpp"

namespace termui {

std::string whitespace(int len) { return std::string(len, ' '); }

std::string div_line(int len) {
  std::string str;
  for (int i = 0; i < len; ++i)
    str += "─";
  return str;
}

std::string line_sep(int len) { return std::string(len, '\n'); }
std::string bold_text(std::string s) { return formats::BOLD + s + formats::BOLD_OFF; }
std::string reverse_video(std::string s) { return formats::REVERSE_VIDEO + s + formats::NONE; }
std::string faint_text(std::string s) { return formats::FAINT + s + formats::NONE; }

std::string ws(int len) /* whitespace */ { return whitespace(len); }
std::string dl(int len) /* dividing line */ { return div_line(len); }
std::string ls(int len) /* line seperation*/ { return line_sep(len); }
std::string bt(std::string s) /* bold text*/ { return bold_text(s); }
std::string rv(std::string s) /*  reverse video*/ { return reverse_video(s); }
std::string ft(std::string s) /*  faint*/ { return faint_text(s); }

Style::Style() {
  fg = "";
  bg = "";
  w = "";
}

Style::Style(std::string foreground, std::string background, std::string weight) {
  fg = foreground;
  bg = background;
  w = weight;
}

std::string Style::render(std::string s) {
  return std::format("{}{}{}{}{}{}{}", fg, bg, w, s, colors::DEFAULT, background::DEFAULT, formats::NONE);
}

RGB::RGB(int red, int green, int blue) {
  r = red;
  g = green;
  b = blue;
}

std::string RGB::fg_ansi() { return std::format("\033[38;2;{};{};{}m", r, g, b); }

std::string RGB::bg_ansi() { return std::format("\33[48;2;{};{};{}m", r, g, b); }

} // namespace termui
