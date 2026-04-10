#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <termui/core/rendering.hpp>
#include <termui/system/mouse.hpp>
#include <termui/util.hpp>
#include <unistd.h>

namespace termui {

class Terminal {
public:
  // enum class OutputMode { Direct, Buffered };

  static Terminal &instance();

  const Renderer &GetRenderer() const;

  void write(const std::string &str) const; // writes to outbuff, not cout directly
  void write(uint row, uint column, const std::string &str) const;
  void flush() const; // flushes outbuff through cout;

  std::string read() const; // read off stdin

  uint width() const;
  uint height() const;

  Color     ForegroundColor();
  Color     BackgroundColor();
  bool      HasDarkBackground();
  ColorMode ColorCapability();

  // void SetOutputMode(OutputMode om);

  // --- Positioning ---
  void MoveCursor(uint row, uint column) const;
  void SaveCursorPosition();
  void RestoreCursorPosition();
  void CursorUp(uint n) const;
  void CursorDown(uint n) const;
  void CursorForward(uint n) const;
  void CursorBack(uint n) const;
  void CursorNextLine(uint n);
  void CursorPrevLine(uint n);

  // --- Screen ---
  void Reset() const;
  void SaveScreen() const;
  void RestoreScreen() const;
  void AltScreen() const;
  void ExitAltScreen() const;
  void ClearScreen() const;
  void ClearScrollback() const;
  void ClearLine();
  void ClearLines();
  void InsertLines(uint n);
  void DeleteLines(uint n);

  void DisableInputBuffering() const;
  void EnableInputBuffering() const;
  void DisableInputEcho() const;
  void EnableInputEcho() const;

  // --- Session ---
  void SetWindowTitle(std::string);
  void SetForegroundColor(Color);
  void SetBackgroundColor(Color);
  void SetCursorColor(Color);
  void ShowCursor() const;
  void HideCursor() const;
  // void Copy(msg?);
  // void CopyPrimary(msg?);
  // void Notify(std::string title, std::string body) const { std::cout << "\x1b]777;notify;" + title + ";" + body + "\x1b\x5c" << std::flush; }
  // void Motify(std::string title, std::string body) const { std::cout << "\x1b]777;notify;" + title + ";" + body + "\x1b\x5c" << std::flush; }

  // --- Mouse ---
  void EnableMousePress(); // enables X10 mouse mode (button presses
  void DisableMousePress();
  void EnableMouseTracking();
  void DisableMouseTracking();

private:
  Terminal();

  ColorMode color_capability;
  // OutputMode output_mode;

  Renderer renderer;

  mutable std::string outbuff;
};

class TermSetup {
  bool input_buffering,        //
      input_echoing,           //
      show_cursor,             //
      alternate_output_buffer, //
      enable_mouse_reporting;  //

public:
  TermSetup();
  TermSetup(bool input_buffering, bool input_echoing, bool show_cursor, bool alternate_output_buffer, bool enable_mouse_reporting);

  void configure();
  void reset(); // sets terminal defaults, not blindly inversing configure
};

namespace TermSetups {
inline TermSetup fullscreen(false, false, false, true, true);
inline TermSetup fullscreen_no_mouse(false, false, false, true, false);
inline TermSetup fullscreen_with_cursor(false, false, true, true, false);
inline TermSetup windowed(false, false, false, false, true);
} // namespace TermSetups

} // namespace termui

#endif
