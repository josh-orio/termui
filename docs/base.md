# termui — Base Classes

Base classes in termui are the fundamental building blocks of larger TUI systems. They encapsulate text representation and input handling.

> **Note:** In a future version of the library, the features of `Text` and `Pager` may be combined into a single object (TBD).

---

## `termui::Text`

Renders a plain styled block of text, optionally constrained to a fixed width and height.

### Constructors

```cpp
Text(const termui::string &str, Style style = Styles::none, uint width = 0, uint height = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `termui::string` | The text content to display. |
| `style` | `Style` | Text/colour style. Defaults to `Styles::none`. |
| `width` | `uint` | Constraining width in columns. `0` means unconstrained. |
| `height` | `uint` | Constraining height in rows. `0` means unconstrained. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the text to a terminal-ready string. Virtual; can be overridden. |
| `resize(uint w, uint h)` | `void` | Resizes the text area. |
| `width()` | `uint` | Returns the current width. |
| `height()` | `uint` | Returns the current height. |

### Notes

- `render()` is declared `virtual` — subclasses may override it.
- `text`, `w`, `h`, and `style` are exposed as `protected` members for use by subclasses.

---

## `termui::PaddedText`

Extends `Text` with configurable padding. Inherits all `Text` behaviour and adds horizontal and vertical padding around the content.

> **Warning:** Ensure `width` and `height` are greater than the configured padding, otherwise the text will not be visible.

### Constructors

```cpp
PaddedText(const termui::string &str, Style style = Styles::none, Padding pad_style = PaddingStyle::label, uint width = 0, uint height = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `termui::string` | The text content to display. |
| `style` | `Style` | Text/colour style. Defaults to `Styles::none`. |
| `pad_style` | `Padding` | Padding configuration. Defaults to `PaddingStyle::label`. |
| `width` | `uint` | Constraining width in columns (must exceed horizontal padding). |
| `height` | `uint` | Constraining height in rows (must exceed vertical padding). |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the padded text block. Overrides `Text::render()`. |
| `resize(uint w, uint h)` | `void` | Resizes the text area. Inherited from `Text`. |
| `hPadding()` | `uint` | Returns the current horizontal padding value. |
| `vPadding()` | `uint` | Returns the current vertical padding value. |

---

## `termui::Input`

Renders an editable input field with separate styles for its value and placeholder text.

### Constructors

```cpp
Input(const termui::string &value, Style valStyle = Styles::none, const termui::string &placeholder = termui::string(""), Style plhStyle = Styles::faint, uint width = 0, uint height = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `termui::string` | The current input value. |
| `valStyle` | `Style` | Style applied to the value text. Defaults to `Styles::none`. |
| `placeholder` | `termui::string` | Placeholder text shown when the value is empty. Defaults to `""`. |
| `plhStyle` | `Style` | Style applied to the placeholder text. Defaults to `Styles::faint`. |
| `width` | `uint` | Constraining width in columns. `0` means unconstrained. |
| `height` | `uint` | Constraining height in rows. `0` means unconstrained. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the input field to a terminal-ready string. |
| `resize(uint w, uint h)` | `void` | Resizes the input area. |
| `width()` | `uint` | Returns the current width. |
| `height()` | `uint` | Returns the current height. |

---

## `termui::Pager`

Renders a scrollable view over a block of text. Tracks a cursor (start line) that can be moved up or down to pan through content that exceeds the display area.

### Constructors

```cpp
Pager(const termui::string &str, Style style = Styles::none, uint width = 0, uint height = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `termui::string` | The full text content to page through. |
| `style` | `Style` | Text/colour style. Defaults to `Styles::none`. |
| `width` | `uint` | Viewport width in columns. `0` means unconstrained. |
| `height` | `uint` | Viewport height in rows. `0` means unconstrained. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the current page (from `cursor` to `cursor + height`) to a terminal-ready string. |
| `resize(uint w, uint h)` | `void` | Resizes the viewport. |
| `width()` | `uint` | Returns the current viewport width. |
| `height()` | `uint` | Returns the current viewport height. |
| `cursor_up(uint count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(uint count = 1)` | `void` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `uint` | Returns the current cursor position (zero-indexed start line). |

### Notes

- The cursor represents the **first visible line** of the content. It is not a text-insertion cursor.
- `cursor_up` and `cursor_down` default to moving one line at a time if `count` is omitted.
