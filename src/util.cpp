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
  size_t visible_chars = 0;
  size_t index = 0;

  while (index < s.size()) {
    unsigned char byte = static_cast<unsigned char>(s[index]);

    // CSI sequence: ESC [
    if (byte == '\e' && index + 1 < s.size() && s[index + 1] == '[') {

      index += 2; // consume ESC [

      // loop skips parameter & intermediate bytes
      while (index < s.size()) {
        unsigned char c = static_cast<unsigned char>(s[index]);

        ++index; // 'consume' the byte

        // check if this byte is a CSI sequence ender (@ <= c <= ~) - (AFAIK, passes unit tests though)
        if (c >= 0x40 && c <= 0x7E) {
          break; // ends byte skipping loop
        }
      }

      continue; // avoids hitting utf counting logic
    }

    // count only UTF-8 leading bytes
    if ((byte & 0xC0) != 0x80) {
      ++visible_chars;
    }

    ++index;
  }

  return visible_chars;
}

size_t max_visible_length(const std::string &s, size_t n) {
  if (n <= 0)
    return 0;
  if (s.empty())
    return 0;

  size_t visible_chars = 0; // num of printed symbols
  size_t index = 0;         // num of bytes passed

  while (index < s.size()) {
    unsigned char byte = static_cast<unsigned char>(s[index]);

    // CSI sequence: ESC [
    if (byte == '\e' && index + 1 < s.size() && s[index + 1] == '[') {

      index += 2; // consume ESC [

      // loop skips parameter & intermediate bytes
      while (index < s.size()) {
        unsigned char c = static_cast<unsigned char>(s[index]);

        ++index; // 'consume' the byte

        // check if this byte is a CSI sequence ender (@ <= c <= ~)
        if (c >= 0x40 && c <= 0x7E) {
          break; // ends byte skipping loop
        }
      }

      continue; // avoids hitting utf counting logic
    }

    // count only UTF-8 leading bytes
    if ((byte & 0xC0) != 0x80) {
      ++visible_chars;

      if (visible_chars > n) {
        return index; // return num of bytes before this symbol was counted
      }
    }

    ++index;
  }

  return s.size(); // return len if less that n symbols can be made
}

size_t reverse_max_visible_length(const std::string &s, size_t n) {
  if (n <= 0)
    return s.size();
  if (s.empty())
    return s.size();

  std::vector<bool> map(s.size(), false);

  bool in_csi = false;
  size_t csi_start;
  size_t index = 0;

  // forward pass to detect and mark any CSI - easier than reverse detecting them
  while (index < s.size()) {
    unsigned char c = static_cast<unsigned char>(s[index]);
    ++index;

    if (!in_csi && c == '\e' && index < s.size() && s[index] == '[') {
      in_csi = true;
      csi_start = index - 1;

      continue;
    }

    if (in_csi && c >= 0x40 && c <= 0x7E) {
      in_csi = false;

      for (size_t i = csi_start; i <= index && i < map.size(); ++i) {
        map[i] = true; // mark CSI bytes
      }

      index = csi_start + 1;

      continue;
    }
  }

  size_t visible_chars = 0; // num of printed symbols
  index = s.size();         // start from the end

  size_t previous_visible = s.size();

  while (index > 0) {
    --index;

    if (map[index]) {
      continue;
    }

    unsigned char byte = static_cast<unsigned char>(s[index]);

    if ((byte & 0xC0) != 0x80) {
      ++visible_chars;

      if (visible_chars == n) {
        return s.size() - index;
      }

      previous_visible = s.size() - index;
    }
  }

  if (map[0]) { // if starting with invisible sequence/char return prvious vis
    return previous_visible;
  }

  return s.size(); // reached the beginning
}

} // namespace termui
