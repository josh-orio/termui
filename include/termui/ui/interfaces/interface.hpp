#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <termui/system/terminal.hpp>

namespace termui {

class Interface {
public:
  enum class State { EXIT, CONTINUE, SELECT };

protected:
  State state;

  TermSetup term_setup;

  Interface();
  Interface(TermSetup ts);
};

} // namespace termui

#endif
