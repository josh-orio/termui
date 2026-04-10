#include <termui/util.hpp>

namespace termui {

namespace ansi {
std::string fg(uint8_t v) { return std::format("\e[38;5;{}m", v); };
std::string fg(uint8_t r, uint8_t g, uint8_t b) { return std::format("\e[38;2;{};{};{}m", r, g, b); };
std::string bg(uint8_t v) { return std::format("\e[48;5;{}m", v); };
std::string bg(uint8_t r, uint8_t g, uint8_t b) { return std::format("\e[48;2;{};{};{}m", r, g, b); };
} // namespace ansi

std::string repeat(const std::string &s, int n) {
  std::string buff;
  for (int i = 0; i < n; i++) {
    buff += s;
  }
  return buff;
}

std::string curs_up(int n) {
  if (n == 0)
    return "";

  return std::format("\e[{}A", n);
}

std::string curs_down(int n) {
  if (n == 0)
    return "";

  return std::format("\e[{}B", n);
}

std::string curs_right(int n) {
  if (n == 0)
    return "";

  return std::format("\e[{}C", n);
}

std::string curs_left(int n) {
  if (n == 0)
    return "";

  return std::format("\e[{}D", n);
}

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

  bool   in_csi = false;
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

std::string color_swatch(uint swatches_per_line) {
  std::string outbuff;

  for (int i = 0; i < 256; i++) {
    // outbuff += termui::bg_apply(std::format("{:4d}", i), i);
    outbuff += std::format("\x1b[48;5;{}m", i) + std::format("{:4d}", i) + "\x1b[0m";
    if (i % swatches_per_line == swatches_per_line - 1) {
      outbuff += '\n';
    }
  }

  return outbuff;
}

std::string test_sgr_features() {
  std::string outbuff;

  for (int i = 0; i < 10; i++) {
    outbuff += std::format("\e[{}mHello\e[0m\n", i);
  }

  return outbuff;
}

std::string inpmap(std::string i) {
  std::string outbuff;

  for (auto c : i) {
    outbuff += std::to_string(static_cast<int>(c)) + " ";
  }

  return outbuff;
}

} // namespace termui
