#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <termui/system/terminal.hpp>

namespace termui {

class Interface {
protected:
  enum class State { EXIT, CONTINUE, SELECT };
  State state;

  TermSetup term_setup;

  Interface();
  Interface(TermSetup ts);
};

} // namespace termui

#endif
