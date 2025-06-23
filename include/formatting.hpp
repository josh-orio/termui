#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <format>
#include <string>

namespace formats {
inline std::string NONE = "\033[0m";
inline std::string BOLD = "\033[1m";
inline std::string FAINT = "\033[2m";
inline std::string ITALIC = "\033[3m";
inline std::string UNDERLINE = "\033[4m";
inline std::string SLOW_BLINK = "\033[5m";
inline std::string RAPID_BLINK = "\033[6m";
inline std::string REVERSE_VIDEO = "\033[7m";
inline std::string CONCEAL = "\033[8m";
inline std::string CROSSED_OUT = "\033[9m";
inline std::string BOLD_OFF = "\033[22m";
inline std::string ITALIC_OFF = "\033[23m";
inline std::string UNDERLINE_OFF = "\033[24m";
inline std::string BLINK_OFF = "\033[25m";
inline std::string REVERSE_VIDEO_OFF = "\033[27m";
inline std::string CONCEAL_OFF = "\033[28m";
inline std::string CROSSED_OUT_OFF = "\033[29m";
} // namespace formats

namespace colors /* strictly text colors */ {
inline std::string BLACK = "\033[30m";
inline std::string RED = "\033[31m";
inline std::string GREEN = "\033[32m";
inline std::string YELLOW = "\033[33m";
inline std::string BLUE = "\033[34m";
inline std::string MAGENTA = "\033[35m";
inline std::string CYAN = "\033[36m";
inline std::string WHITE = "\033[37m";
inline std::string DEFAULT = "\033[39m";
} // namespace colors

namespace background /* or highlighting */ {
inline std::string BLACK = "\033[40m";
inline std::string RED = "\033[41m";
inline std::string GREEN = "\033[42m";
inline std::string YELLOW = "\033[43m";
inline std::string BLUE = "\033[44m";
inline std::string MAGENTA = "\033[45m";
inline std::string CYAN = "\033[46m";
inline std::string WHITE = "\033[47m";
inline std::string DEFAULT = "\033[49m";
} // namespace background

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

class RGB {
  int r, g, b;

public:
  RGB(int red, int green, int blue);

  std::string fg_ansi(); // returns RGB ANSI for text color
  std::string bg_ansi(); // returns RGB ANSI for text background
};

class Style {
public:
  std::string fg, bg, w;

  Style();
  // arguments can be passed in any order, try not to shoot your foot off.
  Style(std::string fg, std::string bg, std::string w);

  std::string render(std::string s);
};

#endif