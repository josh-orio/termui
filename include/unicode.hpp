#ifndef UNICODE_HPP
#define UNICODE_HPP

#include <string>

namespace termui {

namespace unicode {

inline std::string TICK = "✓";
inline std::string DOT = "·";

inline std::string VERTICAL = "│";

inline std::string TOP_LEFT = "┌";
inline std::string TOP_RIGHT = "┐";
inline std::string BOTTOM_LEFT = "└";
inline std::string BOTTOM_RIGHT = "┘";

inline std::string TOP_LEFT_ROUND = "╭";
inline std::string TOP_RIGHT_ROUND = "╮";
inline std::string BOTTOM_LEFT_ROUND = "╰";
inline std::string BOTTOM_RIGHT_ROUND = "╯";

inline std::string ELLIPSIS = "…";
inline std::string L_ARROW = "←";
inline std::string R_ARROW = "→";
inline std::string U_ARROW = "↑";
inline std::string D_ARROW = "↓";
inline std::string HBD = "←/→"; // horizontal bi directional
inline std::string VBD = "↑/↓"; // vertical bi directional
inline std::string ENTER = "↵";
inline std::string TAB = "⇥";

// u2500 to u259f

} // namespace unicode

} // namespace termui

#endif
