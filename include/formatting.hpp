#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include <string>

namespace formats {
inline std::string NONE = "\033[0m";   // Reset all text attributes to default
inline std::string BOLD = "\033[1m";   // Bold on
inline std::string FAINT = "\033[2m";  // Faint off
inline std::string ITALIC = "\033[3m"; // Italic on
inline std::string UNDERLINE = "\033[4m";          // Underline on
inline std::string SLOW_BLINK = "\033[5m";         // Slow blink on
inline std::string RAPID_BLINK = "\033[6m";        // Rapid blink on
inline std::string REVERSE_VIDEO = "\033[7m";      // Reverse video on
inline std::string CONCEAL = "\033[8m";            // Conceal on
inline std::string CROSSED_OUT = "\033[9m";        // Crossed-out on
inline std::string BOLD_OFF = "\033[22m";          // Bold off
inline std::string ITALIC_OFF = "\033[23m";        // Italic off
inline std::string UNDERLINE_OFF = "\033[24m";     // Underline off
inline std::string BLINK_OFF = "\033[25m";         // Blink off
inline std::string REVERSE_VIDEO_OFF = "\033[27m"; // Reverse video off
inline std::string CONCEAL_OFF = "\033[28m";       // Conceal off
inline std::string CROSSED_OUT_OFF = "\033[29m";   // Crossed-out off
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

std::string div_line(int width); // creates a dividing line

std::string bold_text(std::string s);
std::string highlight_text(std::string s);
std::string faint_text(std::string s);

std::string format_text(std::string text, std::string code);
std::string color_text(std::string text, std::string code);
std::string background_text(std::string text, std::string code);

// shorthand utilities
std::string ws(int len);       // whitespace
std::string bt(std::string s); // bold text
std::string ls(int len);       // line seperation

#endif