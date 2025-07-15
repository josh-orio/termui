#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include "ansi.hpp"
#include <format>
#include <string>

namespace termui {

// 8 bit color
std::string fg_color(int c);
std::string bg_color(int c);

// 24 bit color
std::string fg_color(int r, int g, int b);
std::string bg_color(int r, int g, int b);

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

namespace key { /* used for comparison operations when reading off input buffers */
inline std::string ENTER {10};
inline std::string ESC {27};
inline std::string DEL {127};
inline std::string U_ARROW = "\e[A";
inline std::string D_ARROW = "\e[B";
inline std::string R_ARROW = "\e[C";
inline std::string L_ARROW = "\e[D";
} // namespace key

namespace symbol { /* used to make the control strips at the bottom of interfaces */
inline std::string BLOCK = "█";
inline std::string ELLIPSIS ="…";
inline std::string L_ARROW = "←";
inline std::string R_ARROW = "→";
inline std::string U_ARROW = "↑";
inline std::string D_ARROW = "↓";
inline std::string HBD = "←/→"; // horizontal bi directional
inline std::string VBD = "↑/↓"; // vertical bi directional
inline std::string ENTER = "↵";
inline std::string TAB = "⇥";
} // namespace symbol

std::string curs_up(int n);
std::string curs_down(int n);
std::string curs_right(int n);
std::string curs_left(int n);

} // namespace termui

#endif
