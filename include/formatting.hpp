#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include "ansi.hpp"
#include <format>
#include <string>

namespace termui {

std::string whitespace(int len);
std::string div_line(int len);
std::string line_sep(int len);
std::string bold_text(std::string s);
std::string reverse_video(std::string s);
std::string faint_text(std::string s);

// shorthand utilities
std::string ws(int len);       // whitespace
std::string dl(int len);       // dividing line
std::string ls(int len);       // line seperation
std::string bt(std::string s); // bold text
std::string rv(std::string s); // reverse video
std::string ft(std::string s); // faint

class Style {
public:
  std::string foreground, background, weight;

  // arguments can be passed in any order, try not to shoot your foot off.
  Style(std::string f, std::string b, std::string w);

  std::string render(std::string s);
};

// 8 bit color
std::string fg_color(int c);
std::string bg_color(int c);

// 24 bit color
std::string fg_color(int r, int g, int b);
std::string bg_color(int r, int g, int b);

} // namespace termui

#endif
