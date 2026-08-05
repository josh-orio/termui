#include <termui/globals.hpp>

namespace termui {

const Terminal &terminal = Terminal::instance();
const Renderer &renderer = terminal.GetRenderer();

} // namespace termui

namespace tui {

extern const termui::Terminal &term = termui::Terminal::instance();
extern const termui::Renderer &rndr = term.GetRenderer();

} // namespace tui
