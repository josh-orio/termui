#ifndef CONSOLE_HPP
#define CONSOLE_HPP

#include "ansi.hpp"
#include <format>
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace termui {

class BufferModeToggle /* switches between terminal buffering modes */ {
private:
  termios t;

public:
  void off(); // disables buffered input
  void on();  // enables buffered input
};

class EchoModeToggle /* switches between input echo modes */ {
private:
  termios t;

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

class MouseReportingToggle /* enables/disables mouse reporting */ {
public:
  void enable();
  void disable();
};

enum class MouseEventType {
  SCROLL_UP,
  SCROLL_DOWN,
  LEFT_CLICK,     // left button press
  MIDDLE_CLICK,   // middle button press
  RIGHT_CLICK,    // right button press
  BUTTON_RELEASE, // any button release
  MOVE            // use to signal change in mouse pos

  // could also add for SHIFT+click and CTRL+click
};

struct MouseInteraction {
  bool valid;      // indicates whether class was constructed using valid SGR code
  size_t col, row; // on (all ?) terminals, origin (1,1) is top right
  MouseEventType event;

  MouseInteraction(std::string s); // decode from string

  bool match(MouseEventType met);
  bool match(MouseEventType met, size_t x1, size_t x2, size_t y1, size_t y2); // detects if certain mouse event takes place in a region box
  // ^ check implementation to understand how bounds are defined
};

class Console {
public:
  size_t width, height;
  std::string outbuff;

  BufferModeToggle bt;
  EchoModeToggle et;
  CursorModeToggle ct;
  AlternateBufferToggle at;
  MouseReportingToggle mt;

  bool buffered, echos, cursor, altterm, mouserep;

  Console(bool buffered = false, bool echos = false, bool cursor = false, bool alt = true, bool mouse = true);

  void show();
  void close();

  void update_size();

  void print(int row, int col, std::string s);
  void print(std::string s); // only use this if output is or doesnt need to be positionally formatted

  void clear_outbuff();
  void clear_screen();
  void clear_scrollback();
  void flush(bool s_clr = true, bool sb_clr = true); // flush outbuff (w args to clear screen before and clear scrollback)

  std::string poll_input(); // read (upto 16) chars off stdin into inbuff

private:
  winsize w;
};

} // namespace termui

#endif
