#include "util.hpp"

namespace termui {

// apply 8 bit colors
std::string fg_apply(std::string text, int col) {
  if (col != clr::DEFAULT) {
    return std::format("\e[38;5;{}m", col) + text + format::FG_DEFAULT;
  } else {
    return text;
  }
}

std::string bg_apply(std::string text, int col) {
  if (col != clr::DEFAULT) {
    return std::format("\e[48;5;{}m", col) + text + format::BG_DEFAULT;
  } else {
    return text;
  }
}

std::string whitespace(int len) { return std::string(len, ' '); }

std::string div_line(int len) {
  std::string str;
  for (int i = 0; i < len; ++i)
    str += "─";
  return str;
}

std::string line_sep(int len) { return std::string(len, '\n'); }
std::string bold_text(std::string s) { return format::BOLD + s + format::BOLD_OFF; }
std::string reverse_video(std::string s) { return format::REVERSE_VIDEO + s + format::REVERSE_VIDEO_OFF; }
std::string faint_text(std::string s) { return format::FAINT + s + format::FAINT_OFF; }

std::string ws(int len) /* whitespace */ { return whitespace(len); }
std::string dl(int len) /* dividing line */ { return div_line(len); }
std::string ls(int len) /* line seperation*/ { return line_sep(len); }
std::string bt(std::string s) /* bold text*/ { return bold_text(s); }
std::string rv(std::string s) /*  reverse video*/ { return reverse_video(s); }
std::string ft(std::string s) /*  faint*/ { return faint_text(s); }

std::string curs_up(int n) { return std::format("\e[{}A", n); }
std::string curs_down(int n) { return std::format("\e[{}B", n); }
std::string curs_right(int n) { return std::format("\e[{}C", n); }
std::string curs_left(int n) { return std::format("\e[{}D", n); }

} // namespace termui
