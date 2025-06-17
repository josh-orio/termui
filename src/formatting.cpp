#include "formatting.hpp"

std::string div_line(int len) {
  std::string str;
  for (int i = 0; i < len; ++i)
    str += "─";
  return str;
}

std::string bold_text(std::string s) {
  return formats::BOLD + s + formats::BOLD_OFF;
}

std::string highlight_text(std::string s) {
  return formats::REVERSE_VIDEO + s + formats::NONE;
}

std::string faint_text(std::string s) {
  return formats::FAINT + s + formats::NONE;
}

std::string format_text(std::string text, std::string code) {
  return code + text + formats::NONE;
}

std::string color_text(std::string text, std::string code) {
  return code + text + colors::WHITE;
}

std::string background_text(std::string text, std::string code) {
  return code + text + background::DEFAULT;
}

std::string ws(int len) { return std::string(len, ' '); }

std::string bt(std::string s) { return bold_text(s); }

std::string ls(int len) { return std::string(len, '\n'); }
