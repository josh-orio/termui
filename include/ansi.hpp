#ifndef ANSI_HPP
#define ANSI_HPP

#include <string>

namespace termui {

namespace format { /* text formatting codes */
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
} // namespace format

namespace fg { /* text color codes */
inline std::string BLACK = "\033[30m";
inline std::string RED = "\033[31m";
inline std::string GREEN = "\033[32m";
inline std::string YELLOW = "\033[33m";
inline std::string BLUE = "\033[34m";
inline std::string MAGENTA = "\033[35m";
inline std::string CYAN = "\033[36m";
inline std::string WHITE = "\033[37m";
inline std::string DEFAULT = "\033[39m";
} // namespace fg

namespace bg { /* text background color codes */
inline std::string BLACK = "\033[40m";
inline std::string RED = "\033[41m";
inline std::string GREEN = "\033[42m";
inline std::string YELLOW = "\033[43m";
inline std::string BLUE = "\033[44m";
inline std::string MAGENTA = "\033[45m";
inline std::string CYAN = "\033[46m";
inline std::string WHITE = "\033[47m";
inline std::string DEFAULT = "\033[49m";
} // namespace bg

namespace term { /* random terminal manipulation codes */
inline std::string CLEAR_CONSOLE = "\033[2J";
inline std::string CLEAR_SCROLLBACK = "\033[3J";
inline std::string CURSOR_HOME = "\033[H";
inline std::string HIDE_CURSOR = "\033[?25l";
inline std::string SHOW_CURSOR = "\033[?25h";
inline std::string ALT_BUFFER = "\033[?1049h";     // enables the alternative buffer
inline std::string PRIMARY_BUFFER = "\033[?1049l"; // disables the alternative buffer
} // namespace term

} // namespace termui

#endif
