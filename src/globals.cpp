#include <termui/globals.hpp>

namespace termui {
const Terminal &terminal = Terminal::instance();
} // namespace termui

namespace tui {
const termui::Terminal &term = termui::Terminal::instance();
} // namespace tui
