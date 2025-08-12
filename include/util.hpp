#ifndef FORMATTING_HPP
#define FORMATTING_HPP

#include "ansi.hpp"
#include <format>
#include <string>

namespace termui {

namespace clr {          /* common colors and their 8 bit codes*/
inline int DEFAULT = -1; // not a color, just tricks an if
inline int RED = 1;
inline int GREEN = 2;
inline int BLUE = 4;
inline int YELLOW = 3;
inline int ORANGE = 208;
inline int PINK = 219;
inline int MAGENTA = 201;
inline int PURPLE = 56;
inline int LIME = 76;
inline int TEAL = 75;
inline int BLACK = 232;
inline int WHITE = 255;
inline int BLUEPURPLE = 56;
inline int NEONY_PURPLE = 21;
inline int MIDBLUE = 33;
inline int DARKGREY = 238;
inline int LIGHTGREY = 242;
inline int PASTELPINK = 219;
} // namespace clr

// apply 8 bit colors
std::string fg_apply(std::string text, int col);
std::string bg_apply(std::string text, int col);

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

std::string curs_up(int n);
std::string curs_down(int n);
std::string curs_right(int n);
std::string curs_left(int n);

namespace key { /* used for comparison operations when reading off input buffers */
inline std::string ENTER{10};
inline std::string ESC{27};
inline std::string DEL{127};
inline std::string U_ARROW = "\e[A";
inline std::string D_ARROW = "\e[B";
inline std::string R_ARROW = "\e[C";
inline std::string L_ARROW = "\e[D";
} // namespace key

enum class align { NONE, LEFT, RIGHT, CENTER, TOP, BOTTOM };

} // namespace termui

#endif
