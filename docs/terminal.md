# termui — Terminal & TermSetup

`Terminal` is the single point of contact between termui and the real terminal: it knows what the terminal can do (colour depth, size) and is responsible for turning `Style` and cursor/screen operations into the correct ANSI escape sequences. `TermSetup` is a small helper for putting the terminal into (and back out of) a "TUI-ready" state.

---

## `termui::Terminal`

A singleton representing the connected terminal. Higher-level classes (`Base`, `Widget`, `Interface`) talk to `Terminal` instead of touching ANSI codes or `std::cout` directly.

### Access

`Terminal` is a singleton with a private constructor, so it can't be created directly. In application code it's accessed through a global reference rather than by calling `instance()` yourself:

```cpp
#include <termui/globals.hpp>

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
enum class Mode { Ascii, Ansi256, TrueColor };
```

| Mode | Meaning |
|------|---------|
| `Ascii` | No colour support; `Style()` emits nothing. |
| `Ansi256` | 256-colour indexed palette support. |
| `TrueColor` | Full 24-bit RGB colour support. |

The mode is detected once at startup from the `COLORTERM` and `TERM` environment variables.

| Method | Returns | Description |
|--------|---------|-------------|
| `ForegroundColor()` | `Color` | The terminal's current default foreground colour. |
| `BackgroundColor()` | `Color` | The terminal's current default background colour. |
| `HasDarkBackground()` | `bool` | `true` if the terminal's background colour is dark, useful for choosing a light/dark theme automatically. |

### Output

```cpp
void write(const std::string &str) const;
void write(unsigned int row, unsigned int column, const std::string &str) const;
void flush() const;
std::string read() const;
```

| Method | Description |
|--------|-------------|
| `write(str)` | Appends `str` to the internal output buffer. Does not write to the terminal until `flush()` is called. |
| `write(row, column, str)` | Moves the cursor to `(row, column)` and appends `str` to the output buffer. |
| `flush()` | Writes the buffered output to the terminal (`std::cout`) and clears the buffer. |
| `read()` | Reads stdin and returns one key: either a single character, or a whole escape sequence such as an arrow key or mouse event. Returns `""` if nothing complete is available. Compare the result against the codes in `termui::key`. |

### Size

| Method | Returns | Description |
|--------|---------|-------------|
| `width()` | `unsigned int` | Terminal width in columns. |
| `height()` | `unsigned int` | Terminal height in rows. |
| `halfWidth()` | `unsigned int` | Half the terminal width, rounded as implemented. Convenient for centering. |
| `halfHeight()` | `unsigned int` | Half the terminal height, rounded as implemented. Convenient for centering. |

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
void MoveCursor(unsigned int row, unsigned int column) const;
void SaveCursorPosition() const;
void RestoreCursorPosition() const;

std::tuple<int, int> GetCursorPosition() const;
std::tuple<int, int> GetFutureCursorPosition() const;

const Terminal &CursorUp(unsigned int n) const;
const Terminal &CursorDown(unsigned int n) const;
const Terminal &CursorRight(unsigned int n) const;
const Terminal &CursorLeft(unsigned int n) const;
```

| Method | Description |
|--------|-------------|
| `MoveCursor(row, column)` | Moves the cursor to an absolute position. |
| `SaveCursorPosition()` | Saves the current cursor position for later restoration. |
| `RestoreCursorPosition()` | Moves the cursor back to the last saved position. |
| `GetCursorPosition()` | Asks the terminal for the cursor position and waits for the reply. Returns `{row, column}`, ignoring anything still in the output buffer. |
| `GetFutureCursorPosition()` | Returns the `{row, column}` the cursor will be at once the output buffer has been flushed. |
| `CursorUp(n)` / `CursorDown(n)` / `CursorRight(n)` / `CursorLeft(n)` | Moves the cursor `n` cells in the given direction, relative to its current position. Returns `*this` to allow chaining. |

### Screen

```cpp
void ClearScreen() const;
void ClearScrollback() const;
```

| Method | Description |
|--------|-------------|
| `ClearScreen()` | Clears the visible screen. |
| `ClearScrollback()` | Clears the terminal's scrollback history. |

### Session

```cpp
void ShowCursor() const;
void HideCursor() const;
```

| Method | Description |
|--------|-------------|
| `ShowCursor()` | Makes the cursor visible. |
| `HideCursor()` | Hides the cursor. |

### Not yet implemented

The following methods are declared in `terminal.hpp` but have no implementation, so calling one fails at link time.

| Group | Methods |
|-------|---------|
| Color | `ColorCapability()` |
| Positioning | `saveCursor()`, `popCursor()`, `CursorNextLine()`, `CursorPrevLine()` |
| Screen | `Reset()`, `SaveScreen()`, `RestoreScreen()`, `AltScreen()`, `ExitAltScreen()`, `ClearLine()`, `ClearLines()`, `InsertLines()`, `DeleteLines()` |
| Input | `DisableInputBuffering()`, `EnableInputBuffering()`, `DisableInputEcho()`, `EnableInputEcho()` |
| Session | `SetWindowTitle()`, `SetForegroundColor()`, `SetBackgroundColor()`, `SetCursorColor()` |
| Mouse | `EnableMousePress()`, `DisableMousePress()`, `EnableMouseTracking()`, `DisableMouseTracking()` |

Use `TermSetup` to configure input handling, the alternate screen and mouse reporting.

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
| `TermSetup()` | Standard terminal defaults: buffered, echoing input with a visible cursor, on the main screen, without mouse reporting. |
| `TermSetup(input_buffering, input_echoing, show_cursor, alternate_output_buffer, enable_mouse_reporting)` | Explicitly specifies each setting. See parameter table below. |

| Parameter | Type | Description |
|-----------|------|-------------|
| `input_buffering` | `bool` | Whether stdin should be line-buffered. |
| `input_echoing` | `bool` | Whether typed input should be echoed to the terminal. |
| `show_cursor` | `bool` | Whether the cursor should be visible. |
| `alternate_output_buffer` | `bool` | Whether to use the terminal's alternate screen buffer. |
| `enable_mouse_reporting` | `bool` | Whether mouse events should be reported. |

### Presets

| Preset | Buffering | Echo | Cursor | Alternate screen | Mouse |
|--------|-----------|------|--------|------------------|-------|
| `TermSetups::fullscreen` | off | off | hidden | on | on |
| `TermSetups::fullscreen_no_mouse` | off | off | hidden | on | off |
| `TermSetups::fullscreen_with_cursor` | off | off | visible | on | off |
| `TermSetups::windowed` | off | off | hidden | off | on |

### Methods

| Method | Description |
|--------|-------------|
| `configure()` | Applies this `TermSetup`'s settings to the terminal — e.g. switching to the alternate buffer, disabling input echo — to give the application a proper full-screen interface feel. Writes straight to `std::cout`, bypassing the output buffer. |
| `reset()` | Restores the terminal to a normal, usable state. This is a fixed set of sensible defaults, not simply the inverse of `configure()`. |

### Notes

- Typically constructed once at application startup, with `configure()` called before entering the main loop and `reset()` called on exit (including on error paths), so the user's terminal isn't left in a broken state.

---

## `termui::MouseInteraction`

Decodes a mouse event read from `Terminal::read()`. Mouse reporting must be enabled through `TermSetup`.

### Constructors

```cpp
MouseInteraction(std::string s);
```

Decodes the SGR mouse sequence `s`. If `s` isn't a mouse sequence, `valid` is `false` and `match()` always returns `false`.

### Members

| Member | Type | Description |
|--------|------|-------------|
| `valid` | `bool` | Whether `s` was a valid mouse sequence. |
| `col`, `row` | `size_t` | The position of the event. |
| `event` | `EventType` | One of `Move`, `ScrollUp`, `ScrollDown`, `ButtonPress`, `ButtonRelease`. |
| `button` | `MouseButton` | One of `None`, `Left`, `Middle`, `Right`. |
| `modifiers` | `ModifierKey` | Which of `shift`, `control` and `option` were held. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `match(event, button = MouseButton::None, modifiers = Modifiers::None)` | `bool` | `true` if the event, button and modifiers all match exactly. |
| `match(x1, x2, y1, y2, event, button = MouseButton::None, modifiers = Modifiers::None)` | `bool` | As above, and the position is within the region from column `x1` to `x2` and row `y1` to `y2`, inclusive. |
