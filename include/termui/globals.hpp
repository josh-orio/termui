#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <termui/system/terminal.hpp>

namespace termui {

extern const Terminal &terminal;
extern const Renderer &renderer;

} // namespace termui

namespace tui {
inline const termui::Terminal &term = termui::terminal;
inline const termui::Renderer &rndr = termui::renderer;
} // namespace tui

#endif
