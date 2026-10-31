# termui — Display Classes

Display classes in termui are responsible for rendering visual elements in the terminal user interface. They provide building blocks for creating layouts, borders, lines, and progress indicators.

All `render()` methods write through `termui::terminal` into its output buffer. Nothing appears on screen until `terminal.flush()` is called.

---

## `termui::Area`

Renders a filled rectangular area with a configurable symbol pattern.

### Constructors

```cpp
Area(unsigned int width, unsigned int height, Style style, std::string symbol = "╱");
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `width` | `unsigned int` | The width of the area in columns. |
| `height` | `unsigned int` | The height of the area in rows. |
| `style` | `Style` | The style applied to the area. |
| `symbol` | `std::string` | The fill symbol. Defaults to `"╱"`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the area to the terminal buffer. |

### Notes

- The `symbol` parameter allows customization of the fill pattern.
- Common symbols include `"╱"`, `"─"`, `"│"`, `"░"`, `"▒"`, etc.

---

## `termui::Box`

Renders a bordered rectangular box with customizable border styles.

### Constructors

```cpp
Box(unsigned int width, unsigned int height, Border border, Style style = Styles::none);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `width` | `unsigned int` | The width of the box in columns. |
| `height` | `unsigned int` | The height of the box in rows. |
| `border` | `Border` | The border style. See `Borders::square` or `Borders::rounded`. |
| `style` | `Style` | The style applied to the box. Defaults to `Styles::none`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the box to the terminal buffer. |
| `resize(unsigned int w, unsigned int h)` | `void` | Resizes the box. |
| `width()` | `unsigned int` | Returns the current width. |
| `height()` | `unsigned int` | Returns the current height. |

### Notes

- Use `Borders::square` for classic box drawing characters.
- Use `Borders::rounded` for rounded corners.
- The border struct contains strings for all border segments: `Top`, `Bottom`, `Left`, `Right`, `TopLeft`, `TopRight`, `BottomLeft`, `BottomRight`, `MiddleLeft`, `MiddleRight`, `Middle`, `MiddleTop`, `MiddleBottom`.

---

## `termui::Line`

Renders a line of a given length in one of four directions.

### Constructors

```cpp
Line(Direction d, size_t len, std::string symbol = "");
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `d` | `Direction` | The direction the line is drawn in. |
| `len` | `size_t` | The length of the line in cells. |
| `symbol` | `std::string` | The symbol to draw with. If empty, the default for the direction is used. |

### Direction

```cpp
enum class Direction { Horizontal, Vertical, Ascending, Descending };
```

| Value | Default symbol | `tui` shorthand |
|-------|----------------|-----------------|
| `Horizontal` | `─` | `tui::linedir::h` |
| `Vertical` | `│` | `tui::linedir::v` |
| `Ascending` | `╱` | `tui::linedir::a` |
| `Descending` | `╲` | `tui::linedir::d` |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the line to the terminal buffer, starting at the cursor. |
| `style(Style s)` | `Line&` | Sets the style. |

---

## `termui::ProgressBar`

Renders a progress bar with configurable width, completion percentage, and colors.

### Constructors

```cpp
ProgressBar(unsigned int width, float decimal, Color foreground, Color background);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `width` | `unsigned int` | The width of the progress bar in columns. |
| `decimal` | `float` | The completion percentage as a decimal (0.0 to 1.0). |
| `foreground` | `Color` | The foreground color of the progress bar. |
| `background` | `Color` | The background color of the progress bar. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the progress bar to the terminal buffer. |

### Notes

- The `decimal` parameter should be between 0.0 (empty) and 1.0 (full).
- Use `Colors::` namespace for predefined colors or create custom colors.
- The progress bar fills from left to right based on the `decimal` value.

---
