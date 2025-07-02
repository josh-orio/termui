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
std::string bold_text(std::string s) { return format::BOLD + s + format::BOLD_OFF; }
std::string reverse_video(std::string s) { return format::REVERSE_VIDEO + s + format::NONE; }
std::string faint_text(std::string s) { return format::FAINT + s + format::NONE; }

std::string ws(int len) /* whitespace */ { return whitespace(len); }
std::string dl(int len) /* dividing line */ { return div_line(len); }
std::string ls(int len) /* line seperation*/ { return line_sep(len); }
std::string bt(std::string s) /* bold text*/ { return bold_text(s); }
std::string rv(std::string s) /*  reverse video*/ { return reverse_video(s); }
std::string ft(std::string s) /*  faint*/ { return faint_text(s); }

Style::Style(std::string f, std::string b, std::string w) {
  foreground = f;
  background = b;
  weight = w;
}

std::string Style::render(std::string s) {
  return std::format("{}{}{}{}{}", foreground, background, weight, s, format::NONE);
}

// 8 bit color
std::string fg_color(int c) { return std::format("\033[38;5;{}m", c); }
std::string bg_color(int c) { return std::format("\033[48;5;{}m", c); }

// 24 bit color
std::string fg_color(int r, int g, int b) { return std::format("\033[38;2;{};{};{}m", r, g, b); }
std::string bg_color(int r, int g, int b) { return std::format("\33[48;2;{};{};{}m", r, g, b); }

} // namespace termui
