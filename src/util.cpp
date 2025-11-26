#include <termui/util.hpp>

namespace termui {

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

std::string curs_up(int n) { return std::format("\e[{}A", n); }
std::string curs_down(int n) { return std::format("\e[{}B", n); }
std::string curs_right(int n) { return std::format("\e[{}C", n); }
std::string curs_left(int n) { return std::format("\e[{}D", n); }

size_t visible_length(const std::string &s) {
  size_t count = 0;
  for (unsigned char c : s) {
    //  UTF-8 bytes that start with binary 10xxxxxx are continuation bytes, not the start of a new character.
    if ((c & 0xC0) != 0x80) {
      ++count;
    }
  }
  return count;
}

size_t max_visible_length(const std::string &s, int n) {
  size_t count = 0; // num of printed symbols
  size_t pos = 0;   // num of bytes passed

  for (unsigned char c : s) {
    if ((c & 0xC0) != 0x80) {
      ++count;
      if (count > static_cast<size_t>(n)) {
        return pos; // return  num of bytes before this symbol was counted
      }
    }
    ++pos;
  }

  return s.size(); // return len if less that n symbols can be made
}

} // namespace termui
