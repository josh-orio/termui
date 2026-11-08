# termui — Base Classes

Base classes in termui are the fundamental building blocks of larger TUI systems. They encapsulate text representation and input handling.

All `render()` methods write through `termui::terminal` into its output buffer. Nothing appears on screen until `terminal.flush()` is called.

> **Note:** In a future version of the library, the features of `Text` and `Pager` may be combined into a single object (TBD).

---

## `termui::Text`

Renders a styled block of text, optionally constrained to a fixed width and height.

### Constructors

```cpp
Text(const termui::string &str, const Style &style = {}, std::optional<unsigned int> width = std::nullopt, std::optional<unsigned int> height = std::nullopt);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `termui::string` | The text content to display. |
| `style` | `Style` | Text/colour style. Defaults to an inert `Style`, which inherits the active style. |
| `width` | `std::optional<unsigned int>` | Constraining width in columns. Unset means unconstrained. |
| `height` | `std::optional<unsigned int>` | Constraining height in rows. Unset means unconstrained. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the text to the terminal buffer. Virtual; can be overridden. |
| `style(const Style &s)` | `Text&` | Sets the style. |
| `align(const Alignment &a)` | `Text&` | Sets the alignment. Defaults to `Alignment::Left`. |
| `width(std::optional<unsigned int> w)` | `Text&` | Sets or clears the width. |
| `height(std::optional<unsigned int> h)` | `Text&` | Sets or clears the height. |
| `size(std::optional<unsigned int> w, std::optional<unsigned int> h)` | `Text&` | Sets or clears both at once. |
| `size()` | `std::pair<unsigned int, unsigned int>` | Measures the rendered text, returned as `{width, height}`. Returns `{0, 0}` if there is nothing to draw. |

### Alignment

```cpp
enum class Alignment { Left, Center, Right };
```

### Notes

- An unset size and a size of `0` are different: unset means unconstrained, while `0` means nothing is drawn.
- Lines wrap at the last space that fits, or mid-word if there is no space. `\n` always starts a new line.
- With a height set, the output is never taller than `height`. If text remains after the last line, it is cut off and ends in `…`. Trailing newlines alone don't count as remaining text.
- With a height set, unused rows are filled with blank lines.
- With a width set, each line is padded to `width` and aligned using its visible width, so UTF-8 and coloured text line up correctly. Without a width, lines are not padded or aligned.
- `render()` is declared `virtual` — subclasses may override it.
- `_text`, `_style`, `_align`, `_w`, `_h`, and `layout()` are `protected` for use by subclasses. `layout()` returns the wrapped, truncated and aligned lines.

---

## `termui::TextRow`

Lays out several `Text` segments side by side as columns that share one width and height, separated by padding. Segments are filled left to right; the segment that runs out of room is cut with an ellipsis, and any after it are not drawn.

### Constructors

```cpp
TextRow(std::optional<unsigned int> width = std::nullopt, std::optional<unsigned int> height = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `width` | `std::optional<unsigned int>` | Total width shared by the segments. Unset means each segment takes its natural width. |
| `height` | `std::optional<unsigned int>` | Height of every column. Defaults to `1`. Unset fits the tallest segment. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `add(const Text &text, unsigned int reserve = 0)` | `TextRow&` | Appends a segment. `reserve` is the width set aside for it, which earlier segments can't use. |
| `width(std::optional<unsigned int> w)` | `TextRow&` | Sets or clears the width. |
| `height(std::optional<unsigned int> h)` | `TextRow&` | Sets or clears the height. |
| `size(std::optional<unsigned int> w, std::optional<unsigned int> h)` | `TextRow&` | Sets or clears both at once. |
| `padding(unsigned int p)` | `TextRow&` | Sets the number of empty cells between segments. Defaults to `1`. |
| `size()` | `std::pair<unsigned int, unsigned int>` | Measures the rendered row, returned as `{width, height}`. Returns `{0, 0}` if there is nothing to draw. |
| `render()` | `void` | Writes the row to the terminal buffer, leaving the cursor on its bottom row, after its last column. |

### Example

```cpp
termui::TextRow(w)
    .add(termui::Text(name, bold))
    .add(termui::Text("referenced this job in commit", style))
    .add(termui::Text(hash.substr(0, 7), bold))
    .add(termui::Text(date, style).align(termui::Text::Alignment::Right), 20)
    .render();
```

### Notes

- Each segment keeps its own style and alignment. Any width or height set on it is ignored; the row sizes it.
- A segment's natural width is its widest line.
- Each segment wraps within its own column like `Text`, and is cut with `…` on the last row if text remains.
- With a width set, the last segment takes all leftover width, so a right aligned last segment sits against the right edge.
- A reserve larger than the width is capped at the width.
- Padding only goes between segments that are drawn, and counts towards the width. A reserved segment's padding is held back along with its reserve.
- Padding cells are blank and use the surrounding style, not a segment's style.

---

## `termui::PaddedText`

Extends `Text` with configurable padding. Inherits all `Text` behaviour and adds horizontal and vertical padding around the content.

> **Warning:** Both `width` and `height` must be set before calling `render()`, and must be greater than the configured padding, otherwise the text will not be visible.

### Constructors

```cpp
PaddedText(const termui::string &str, Style style = {}, Padding pad_style = {});
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `termui::string` | The text content to display. |
| `style` | `Style` | Text/colour style. Defaults to an inert `Style`. |
| `pad_style` | `Padding` | Padding configuration. Defaults to no padding. |

### Padding

```cpp
struct Padding {
  unsigned int left, right, top, bottom;
};
```

| Preset | Left/right | Top/bottom |
|--------|------------|------------|
| `PaddingStyle::none` | `0` | `0` |
| `PaddingStyle::label` | `1` | `0` |
| `PaddingStyle::one` | `1` | `1` |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Draws the padded background, then the text inside it. Overrides `Text::render()`. |
| `setPadding(Padding p)` | `PaddedText&` | Sets the padding. |
| `hPadding()` | `unsigned int` | Returns the total horizontal padding (`left + right`). |
| `vPadding()` | `unsigned int` | Returns the total vertical padding (`top + bottom`). |

`style()`, `align()`, `width()`, `height()` and `size()` are inherited from `Text`.

### Notes

- The inherited setters return `Text&`, so call `setPadding()` before them when chaining.

---

## `termui::Input`

Renders an editable input field with separate styles for its value and placeholder text. `Input` only draws the field; reading keys and updating the value is up to the caller.

### Constructors

```cpp
Input(const termui::string &value = "", const termui::string &placeholder = "", const Style &valStyle = {}, const Style &plhStyle = Styles::faint);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `value` | `termui::string` | The current input value. |
| `placeholder` | `termui::string` | Placeholder text shown when the value is empty. Defaults to `""`. |
| `valStyle` | `Style` | Style applied to the value text. Defaults to an inert `Style`. |
| `plhStyle` | `Style` | Style applied to the placeholder text. Defaults to `Styles::faint`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the input field to the terminal buffer. |
| `value(const termui::string &v)` | `Input&` | Sets the value. |
| `placeholder(const termui::string &p)` | `Input&` | Sets the placeholder. |
| `valueStyle(const Style &s)` | `Input&` | Sets the value style. |
| `placeholderStyle(const Style &s)` | `Input&` | Sets the placeholder style. |
| `width(unsigned int w)` | `Input&` | Sets the width. |
| `height(unsigned int h)` | `Input&` | Sets the height. |
| `size(unsigned int w, unsigned int h)` | `Input&` | Sets both at once. |
| `focus()` | `Input&` | Marks the input as focussed. |
| `blur()` | `Input&` | Marks the input as not focussed. |
| `focussed(bool x)` | `Input&` | Sets the focus state. |
| `get_value()` | `const termui::string&` | Returns the current value. |

### Notes

- Set both `width` and `height` before calling `render()`.
- Set the focus state with `focus()`, `blur()` or `focussed()` before rendering; it has no default.
- When focussed, `render()` leaves the cursor where the next character will appear. A placeholder leaves it at the start of the field.
- With a `height` of `1`, a value that is too long scrolls: the last `width - 1` characters are shown, keeping one cell free for the cursor. A `width` of `1` shows only the cursor cell.
- With a greater `height`, a value wraps like `Text`, and the last `height` lines are shown so that the end of the value stays visible.
- A placeholder that is too long is cut off at the end of the last line.
- Colour codes in a part of the value that has scrolled away still apply to the part that is shown.

---

## `termui::Pager`

Renders a scrollable view over a block of text. Tracks a cursor (start line) that can be moved up or down to pan through content that exceeds the display area. Inherits from `Text`.

### Constructors

```cpp
Pager(const termui::string &str, const Style &style = {}, unsigned int width = 0, unsigned int height = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `str` | `termui::string` | The full text content to page through. |
| `style` | `Style` | Text/colour style. Defaults to an inert `Style`. |
| `width` | `unsigned int` | Viewport width in columns. |
| `height` | `unsigned int` | Viewport height in rows. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the current page (from `cursor` to `cursor + height`) to the terminal buffer. |
| `cursor_up(unsigned int count = 1)` | `Pager&` | Moves the cursor up by `count` lines. |
| `cursor_down(unsigned int count = 1)` | `Pager&` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `unsigned int` | Returns the current cursor position (zero-indexed start line). |

`style()`, `align()`, `width()`, `height()` and `size()` are inherited from `Text`.

### Notes

- A `width` or `height` of `0` draws nothing, so set both, either here or with the inherited setters.
- The cursor represents the **first visible line** of the content. It is not a text-insertion cursor.
- `cursor_up` and `cursor_down` default to moving one line at a time if `count` is omitted.
- `cursor_down` is not bounds checked when called; the value is clamped in `render()`.
