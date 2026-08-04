#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <termui/system/terminal.hpp>

namespace termui {

extern const Terminal &terminal;
extern const Renderer &renderer;

} // namespace termui

namespace tui {
    
extern const termui::Terminal &term;
extern const termui::Renderer &rndr;

} // namespace tui

#endif
