#ifndef ANSI_HPP
#define ANSI_HPP

#include <string>

namespace termui {

namespace format { /* text formatting codes */
inline std::string NONE = "\e[0m";
inline std::string BOLD = "\e[1m";
inline std::string FAINT = "\e[2m";
inline std::string ITALIC = "\e[3m";
inline std::string UNDERLINE = "\e[4m";
inline std::string SLOW_BLINK = "\e[5m";
inline std::string RAPID_BLINK = "\e[6m";
inline std::string REVERSE_VIDEO = "\e[7m";
inline std::string CONCEAL = "\e[8m";
inline std::string CROSSED_OUT = "\e[9m";
inline std::string BOLD_OFF = "\e[22m";
inline std::string FAINT_OFF = "\e[22m"; // two codes do the same thing?
inline std::string ITALIC_OFF = "\e[23m";
inline std::string UNDERLINE_OFF = "\e[24m";
inline std::string BLINK_OFF = "\e[25m";
inline std::string REVERSE_VIDEO_OFF = "\e[27m";
inline std::string CONCEAL_OFF = "\e[28m";
inline std::string CROSSED_OUT_OFF = "\e[29m";
} // namespace format

namespace fg { /* text color codes */
inline std::string BLACK = "\e[30m";
inline std::string RED = "\e[31m";
inline std::string GREEN = "\e[32m";
inline std::string YELLOW = "\e[33m";
inline std::string BLUE = "\e[34m";
inline std::string MAGENTA = "\e[35m";
inline std::string CYAN = "\e[36m";
inline std::string WHITE = "\e[37m";
inline std::string DEFAULT = "\e[39m";
} // namespace fg

namespace bg { /* text background color codes */
inline std::string BLACK = "\e[40m";
inline std::string RED = "\e[41m";
inline std::string GREEN = "\e[42m";
inline std::string YELLOW = "\e[43m";
inline std::string BLUE = "\e[44m";
inline std::string MAGENTA = "\e[45m";
inline std::string CYAN = "\e[46m";
inline std::string WHITE = "\e[47m";
inline std::string DEFAULT = "\e[49m";
} // namespace bg

namespace term { /* random terminal manipulation codes */
inline std::string CLEAR_CONSOLE = "\e[2J";
inline std::string CLEAR_SCROLLBACK = "\e[3J";
inline std::string CURSOR_HOME = "\e[H";
inline std::string HIDE_CURSOR = "\e[?25l";
inline std::string SHOW_CURSOR = "\e[?25h";
inline std::string ALT_BUFFER = "\e[?1049h";     // enables the alternative buffer
inline std::string PRIMARY_BUFFER = "\e[?1049l"; // disables the alternative buffer
} // namespace term

} // namespace termui

#endif
