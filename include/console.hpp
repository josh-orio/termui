#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "ansi.hpp"
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace termui {

namespace keys {
inline constexpr char ENTER = 10;
inline constexpr char ESC = 27;
inline constexpr char D_ARROW = 'B';
inline constexpr char U_ARROW = 'A';
inline constexpr char L_ARROW = 'D';
inline constexpr char R_ARROW = 'C';
inline constexpr char DEL = 127;
} // namespace keys

class BufferModeToggle /* switches between terminal buffering modes */ {
private:
  struct termios t;

public:
  void off(); // disables buffered input
  void on();  // enables buffered input
};

class EchoModeToggle /* switches between input echo modes*/ {
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

class AlternateTerminalToggle /* switches between alt and main terminal buffer */ {
public:
  void enable();  // switches to alternate terminal buffer
  void disable(); // switches back to primary terminal buffer
};

class Console {
public:
  int width, height;

  std::vector<std::string> buffer;

  BufferModeToggle bt;
  EchoModeToggle et;
  CursorModeToggle ct;
  AlternateTerminalToggle at;

  bool buffered, echos, cursor, altterm;

  Console();
  Console(bool buffered, bool echos, bool cursor, bool alt);

  void show();
  void close();

  void clear_buffer();
  void clear_screen();
  void clear_scrollback();

  void print(int row, int col, std::string s);
  void flush();

  void update_size();
};

} // namespace termui

#endif
