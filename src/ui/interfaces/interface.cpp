#include <termui/ui/ui.hpp>

namespace termui {

Interface::Interface() : state(), term_setup() {}
Interface::Interface(TermSetup ts) : state(), term_setup(ts) {}

} // namespace termui
