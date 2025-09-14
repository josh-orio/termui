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

std::string horizontal_line(int len, int col) {
  std::string str;
  for (int i = 0; i < len; ++i) {
    str += "─";
  }

  if (col != clr::DEFAULT) {
    str = fg_apply(str, col);
  }

  return str;
}

std::string vertical_line(int len, int col) {
  std::string str;
  for (int i = 0; i < len; ++i) {
    str += unicode::VERTICAL + curs_down(1) + curs_left(1);
  }

  if (col != clr::DEFAULT) {
    str = fg_apply(str, col);
  }

  return str;
}

std::string line_sep(int len) { return std::string(len, '\n'); }
std::string bold_text(std::string s) { return format::BOLD + s + format::BOLD_OFF; }
std::string reverse_video(std::string s) { return format::REVERSE_VIDEO + s + format::REVERSE_VIDEO_OFF; }
std::string faint_text(std::string s) { return format::FAINT + s + format::FAINT_OFF; }

std::string ws(int len) /* whitespace */ { return whitespace(len); }
std::string hl(int len, int col) /* horizontal line */ { return horizontal_line(len, col); }
std::string vl(int len, int col) /* vertical line */ { return vertical_line(len, col); }
std::string ls(int len) /* line seperation*/ { return line_sep(len); }
std::string bt(std::string s) /* bold text*/ { return bold_text(s); }
std::string rv(std::string s) /*  reverse video*/ { return reverse_video(s); }
std::string ft(std::string s) /*  faint*/ { return faint_text(s); }

std::string curs_up(int n) { return std::format("\e[{}A", n); }
std::string curs_down(int n) { return std::format("\e[{}B", n); }
std::string curs_right(int n) { return std::format("\e[{}C", n); }
std::string curs_left(int n) { return std::format("\e[{}D", n); }

} // namespace termui
