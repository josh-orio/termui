#include <termui/globals.hpp>

namespace termui {

const Terminal &terminal = Terminal::instance();
const Renderer &renderer = terminal.GetRenderer();

} // namespace termui
