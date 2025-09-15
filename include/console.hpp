#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "ansi.hpp"
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace termui {

class BufferModeToggle /* switches between terminal buffering modes */ {
private:
  struct termios t;

public:
  void off(); // disables buffered input
  void on();  // enables buffered input
};

class EchoModeToggle /* switches between input echo modes */ {
private:
  struct termios t;

public:
  void off(); // disables input echoing
  void on();  // enables input echoing
};

class CursorModeToggle /* switches cursor on and off */ {
public:
  void off(); // hides the cursor
  void on();  // enables the cursor
};

class AlternateBufferToggle /* switches between alt and main terminal buffer */ {
public:
  void enable();  // switches to alternate terminal buffer
  void disable(); // switches back to primary terminal buffer
};

class Console {
public:
  int width, height;
  std::string outbuff;

  BufferModeToggle bt;
  EchoModeToggle et;
  CursorModeToggle ct;
  AlternateBufferToggle at;

  bool buffered, echos, cursor, altterm;

  Console(bool buffered = false, bool echos = false, bool cursor = false, bool alt = true);

  void show();
  void close();

  void update_size();

  void print(int row, int col, std::string s);
  void print(std::string s); // only use this if output is or doesnt need to be positionally formatted

  void clear_outbuff();
  void clear_screen();
  void clear_scrollback();
  void flush(bool s_clr = true, bool sb_clr = true); // flush outbuff (w args to clear screen before and clear scrollback)

  std::string poll_input(); // read (upto 8) chars off stdin into inbuff
};

} // namespace termui

#endif
