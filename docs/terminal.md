# termui — Terminal & TermSetup

`Terminal` is the single point of contact between termui and the real terminal: it knows what the terminal can do (colour depth, size) and is responsible for turning `Style` and cursor/screen operations into the correct ANSI escape sequences. `TermSetup` is a small helper for putting the terminal into (and back out of) a "TUI-ready" state.

---

## `termui::Terminal`

A singleton representing the connected terminal. Higher-level classes (`Base`, `Widget`, `Interface`) talk to `Terminal` instead of touching ANSI codes or `std::cout` directly.

### Access

`Terminal` is a singleton with a private constructor, so it can't be created directly. In application code it's accessed through a global reference rather than by calling `instance()` yourself:

```cpp
#include <termui/system/globals.hpp>

termui::terminal.write("hello");
// or, equivalently:
tui::term.write("hello");
```

| Global | Type | Description |
|--------|------|-------------|
| `termui::terminal` | `const Terminal&` | Reference to the single shared `Terminal` instance. |
| `tui::term` | `const termui::Terminal&` | Shorter alias for `termui::terminal`, for use under the `tui` namespace. |

| Method | Returns | Description |
|--------|---------|-------------|
| `Terminal::instance()` | `Terminal&` | Returns the single shared `Terminal` instance. This is what `termui::terminal` and `tui::term` are bound to internally — application code should generally prefer those globals over calling `instance()` directly. |

### Color capability

```cpp
enum class Mode { ASCII, ANSI256, TRUECOLOR };
```

| Mode | Meaning |
|------|---------|
| `ASCII` | No colour support; styling is limited to plain text. |
| `ANSI256` | 256-colour indexed palette support. |
| `TRUECOLOR` | Full 24-bit RGB colour support. |

| Method | Returns | Description |
|--------|---------|-------------|
| `ColorCapability()` | `Color::Mode` | The colour depth the terminal supports. |
| `ForegroundColor()` | `Color` | The terminal's current default foreground colour. |
| `BackgroundColor()` | `Color` | The terminal's current default background colour. |
| `HasDarkBackground()` | `bool` | `true` if the terminal's background colour is dark, useful for choosing a light/dark theme automatically. |

### Output

```cpp
void write(const std::string &str) const;
void write(uint row, uint column, const std::string &str) const;
void flush() const;
std::string read() const;
```

| Method | Description |
|--------|-------------|
| `write(str)` | Appends `str` to the internal output buffer. Does not write to the terminal until `flush()` is called. |
| `write(row, column, str)` | Moves the cursor to `(row, column)` and appends `str` to the output buffer. |
| `flush()` | Writes the buffered output to the terminal (`std::cout`) and clears the buffer. |
| `read()` | Reads and returns a line of input from stdin. |

### Size

| Method | Returns | Description |
|--------|---------|-------------|
| `width()` | `uint` | Terminal width in columns. |
| `height()` | `uint` | Terminal height in rows. |
| `halfWidth()` | `uint` | Half the terminal width, rounded as implemented. Convenient for centering. |
| `halfHeight()` | `uint` | Half the terminal height, rounded as implemented. Convenient for centering. |

### Styling

```cpp
void Style(termui::Style s) const;
void StyleStack(termui::Style s) const;
void StylePop() const;
```

| Method | Description |
|--------|-------------|
| `Style(s)` | Emits the ANSI codes for `s` immediately, as a one-off. Does not affect the style stack. |
| `StyleStack(s)` | Pushes `s` onto an internal style stack and applies it. Pair with `StylePop()` to restore the previous style afterwards. |
| `StylePop()` | Pops the most recently pushed style off the stack and re-applies whatever style is now on top (or resets styling if the stack is empty). |

### Positioning

```cpp
void MoveCursor(uint row, uint column) const;
void SaveCursorPosition();
void RestoreCursorPosition();

const Terminal &CursorUp(uint n) const;
const Terminal &CursorDown(uint n) const;
const Terminal &CursorRight(uint n) const;
const Terminal &CursorLeft(uint n) const;

void CursorNextLine(uint n);
void CursorPrevLine(uint n);
```

| Method | Description |
|--------|-------------|
| `MoveCursor(row, column)` | Moves the cursor to an absolute position. |
| `SaveCursorPosition()` | Saves the current cursor position for later restoration. |
| `RestoreCursorPosition()` | Moves the cursor back to the last saved position. |
| `CursorUp(n)` / `CursorDown(n)` / `CursorRight(n)` / `CursorLeft(n)` | Moves the cursor `n` cells in the given direction, relative to its current position. Returns `*this` to allow chaining. |
| `CursorNextLine(n)` | Moves the cursor down `n` lines, to the start of the line. |
| `CursorPrevLine(n)` | Moves the cursor up `n` lines, to the start of the line. |

### Screen

```cpp
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
```

| Method | Description |
|--------|-------------|
| `Reset()` | Resets terminal styling/state to defaults. |
| `SaveScreen()` | Saves the current screen contents. |
| `RestoreScreen()` | Restores a previously saved screen. |
| `AltScreen()` | Switches to the terminal's alternate screen buffer. |
| `ExitAltScreen()` | Switches back to the main screen buffer. |
| `ClearScreen()` | Clears the visible screen. |
| `ClearScrollback()` | Clears the terminal's scrollback history. |
| `ClearLine()` | Clears the current line. |
| `ClearLines()` | Clears all lines. |
| `InsertLines(n)` | Inserts `n` blank lines at the cursor's position, shifting existing lines down. |
| `DeleteLines(n)` | Deletes `n` lines starting at the cursor's position, shifting subsequent lines up. |
| `DisableInputBuffering()` / `EnableInputBuffering()` | Toggles line buffering on stdin (i.e. raw vs. canonical mode), controlling whether input is delivered a line at a time or a keypress at a time. |
| `DisableInputEcho()` / `EnableInputEcho()` | Toggles whether typed input is echoed back to the terminal. |

### Session

```cpp
void SetWindowTitle(std::string);
void SetForegroundColor(Color);
void SetBackgroundColor(Color);
void SetCursorColor(Color);
void ShowCursor() const;
void HideCursor() const;
```

| Method | Description |
|--------|-------------|
| `SetWindowTitle(title)` | Sets the terminal window/tab title. |
| `SetForegroundColor(c)` | Sets the terminal's default foreground colour. |
| `SetBackgroundColor(c)` | Sets the terminal's default background colour. |
| `SetCursorColor(c)` | Sets the cursor's colour. |
| `ShowCursor()` | Makes the cursor visible. |
| `HideCursor()` | Hides the cursor. |

### Mouse

```cpp
void EnableMousePress();
void DisableMousePress();
void EnableMouseTracking();
void DisableMouseTracking();
```

| Method | Description |
|--------|-------------|
| `EnableMousePress()` / `DisableMousePress()` | Enables/disables X10 mouse mode, which reports mouse button presses only. |
| `EnableMouseTracking()` / `DisableMouseTracking()` | Enables/disables full mouse tracking, additionally reporting mouse motion/drag events. |

### Notes

- `Terminal` is a singleton — access it via the `termui::terminal` / `tui::term` global references rather than constructing one; the constructor is private.
- `write()` and `write(row, column, ...)` buffer output internally (`_outbuff`); nothing reaches the terminal until `flush()` is called.
- `CursorUp`/`Down`/`Right`/`Left` return `const Terminal&` so calls can be chained, e.g. `term.CursorUp(2).CursorRight(4)`.
- `Style`/`StyleStack`/`StylePop` are `const`, but `_style_stack` is `mutable` to allow this — pushing/popping styles doesn't otherwise change the terminal's observable state.

---

## `termui::TermSetup`

A small configuration object for putting the terminal into a "TUI-ready" state and returning it to normal afterwards. `configure()` and `reset()` are independent of one another — `reset()` restores standard terminal defaults regardless of what `configure()` last did, rather than undoing it flag-by-flag.

### Constructors

```cpp
TermSetup();
TermSetup(bool input_buffering, bool input_echoing, bool show_cursor, bool alternate_output_buffer, bool enable_mouse_reporting);
```

| Constructor | Description |
|-------------|-------------|
| `TermSetup()` | Default configuration. |
| `TermSetup(input_buffering, input_echoing, show_cursor, alternate_output_buffer, enable_mouse_reporting)` | Explicitly specifies each setting. See parameter table below. |

| Parameter | Type | Description |
|-----------|------|-------------|
| `input_buffering` | `bool` | Whether stdin should be line-buffered. |
| `input_echoing` | `bool` | Whether typed input should be echoed to the terminal. |
| `show_cursor` | `bool` | Whether the cursor should be visible. |
| `alternate_output_buffer` | `bool` | Whether to use the terminal's alternate screen buffer. |
| `enable_mouse_reporting` | `bool` | Whether mouse events should be reported. |

### Methods

| Method | Description |
|--------|-------------|
| `configure()` | Applies this `TermSetup`'s settings to the terminal — e.g. switching to the alternate buffer, disabling input echo — to give the application a proper full-screen interface feel. |
| `reset()` | Restores the terminal to a normal, usable state. This is a fixed set of sensible defaults, not simply the inverse of `configure()`. |

### Notes

- Typically constructed once at application startup, with `configure()` called before entering the main loop and `reset()` called on exit (including on error paths), so the user's terminal isn't left in a broken state.
