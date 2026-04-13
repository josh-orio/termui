#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>
#include <format>
#include <map>
#include <string>
#include <vector>

namespace termui {
std::string curs_up(int n);
std::string curs_down(int n);
std::string curs_right(int n);
std::string curs_left(int n);

std::string repeat(const std::string &s, int n);

size_t visible_length(const std::string &s);                       // calculates length of printed string (not bytes)
size_t max_visible_length(const std::string &s, size_t n);         // calculates num of bytes that make (up to) n chars when printed
size_t reverse_max_visible_length(const std::string &s, size_t n); // same as ^, but counts from the back of the string

std::string color_swatch(uint swatches_per_line); // print tiles of 8 bit colors on your terminal
std::string test_sgr_features();                  // generates tiles which apply a different sgr text attribute - used to test your terminal

std::string inpmap(std::string i); // print string in bytes

namespace key { /* used for comparison operations when reading off input buffers */
inline std::string ENTER{10};
inline std::string ESC{27};
inline std::string DEL{127};

inline std::string U_ARROW = "\x1b[A";
inline std::string D_ARROW = "\x1b[B";
inline std::string R_ARROW = "\x1b[C";
inline std::string L_ARROW = "\x1b[D";

inline std::string SHIFT_U_ARROW{27, 91, 49, 59, 50, 65}; // shift + arrow
inline std::string SHIFT_D_ARROW{27, 91, 49, 59, 50, 66};
inline std::string SHIFT_R_ARROW{27, 91, 49, 59, 50, 67};
inline std::string SHIFT_L_ARROW{27, 91, 49, 59, 50, 68};
} // namespace key

namespace unicode { /* used to compose interfaces and elements mostly */

inline std::string TICK = "✓";
inline std::string DOT = "·";

inline std::string VERTICAL = "│";
inline std::string VERTICAL_WIDE = "┃";

inline std::string ELLIPSIS = "…";
inline std::string L_ARROW = "←";
inline std::string R_ARROW = "→";
inline std::string U_ARROW = "↑";
inline std::string D_ARROW = "↓";
inline std::string ENTER = "↵";
inline std::string TAB = "⇥";
inline std::string ESC = "␛";

inline std::string FULL_SHADE = "█";
inline std::string DARK_SHADE = "▓";
inline std::string MEDIUM_SHADE = "▒";
inline std::string LIGHT_SHADE = "░";

// Block chars
// U+258x 	▀ 	▁ 	▂ 	▃ 	▄ 	▅ 	▆ 	▇ 	█ 	▉ 	▊ 	▋ 	▌ 	▍ 	▎ 	▏
// U+259x 	▐ 	░ 	▒ 	▓ 	▔ 	▕ 	▖ 	▗ 	▘ 	▙ 	▚ 	▛ 	▜ 	▝ 	▞ 	▟

} // namespace unicode

namespace term { /* random terminal manipulation codes */
inline std::string CLEAR_CONSOLE = "\x1b[2J";
inline std::string CLEAR_SCROLLBACK = "\x1b[3J";
inline std::string CURSOR_HOME = "\x1b[H";
inline std::string HIDE_CURSOR = "\x1b[?25l";
inline std::string SHOW_CURSOR = "\x1b[?25h";
inline std::string ALT_BUFFER = "\x1b[?1049h";     // enables the alternative buffer
inline std::string PRIMARY_BUFFER = "\x1b[?1049l"; // disables the alternative buffer

inline std::string SAVE_CURSOR = "\0337\x1b[s"; // uses dec and xterm code for compat
inline std::string RESTORE_CURSOR = "\0338\x1b[u";

inline std::string ENABLE_MOUSE_REPORTING = "\x1b[?1003h\x1b[?1006h"; // using SGR, X10 is limited
inline std::string DISABLE_MOUSE_REPORTING = "\x1b[?1003l\x1b[?1006l";

} // namespace term

} // namespace termui

#endif
