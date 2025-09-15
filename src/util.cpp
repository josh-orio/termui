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

std::string ws(int len) { return whitespace(len); }                    // whitespace
std::string hl(int len, int col) { return horizontal_line(len, col); } // horizontal line
std::string vl(int len, int col) { return vertical_line(len, col); }   // vertical line
std::string ls(int len) { return line_sep(len); }                      // line seperation
std::string bt(std::string s) { return bold_text(s); }                 // bold text
std::string rv(std::string s) { return reverse_video(s); }             // reverse video
std::string ft(std::string s) { return faint_text(s); }                // faint text

std::string curs_up(int n) { return std::format("\e[{}A", n); }
std::string curs_down(int n) { return std::format("\e[{}B", n); }
std::string curs_right(int n) { return std::format("\e[{}C", n); }
std::string curs_left(int n) { return std::format("\e[{}D", n); }

} // namespace termui
