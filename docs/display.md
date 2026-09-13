# termui — Display Classes

Display classes in termui are responsible for rendering visual elements in the terminal user interface. They provide building blocks for creating layouts, borders, lines, and progress indicators.

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
| `render()` | `std::string` | Renders the area to a terminal-ready string. |
| `resize(unsigned int w, unsigned int h)` | `void` | Resizes the area. |
| `width()` | `unsigned int` | Returns the current width. |
| `height()` | `unsigned int` | Returns the current height. |

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
| `render()` | `std::string` | Renders the box to a terminal-ready string. |
| `resize(unsigned int w, unsigned int h)` | `void` | Resizes the box. |
| `width()` | `unsigned int` | Returns the current width. |
| `height()` | `unsigned int` | Returns the current height. |

### Notes

- Use `Borders::square` for classic box drawing characters.
- Use `Borders::rounded` for rounded corners.
- The border struct contains strings for all border segments: `Top`, `Bottom`, `Left`, `Right`, `TopLeft`, `TopRight`, `BottomLeft`, `BottomRight`, `MiddleLeft`, `MiddleRight`, `Middle`, `MiddleTop`, `MiddleBottom`.

---

## `termui::HorizontalLine`

Renders a horizontal line of a specified width.

### Constructors

```cpp
HorizontalLine(unsigned int width, Border border);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `width` | `unsigned int` | The width of the line in columns. |
| `border` | `Border` | The border style for the line. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the horizontal line to a terminal-ready string. |
| `width()` | `unsigned int` | Returns the current width. |

---

## `termui::VerticalLine`

Renders a vertical line of a specified height.

### Constructors

```cpp
VerticalLine(unsigned int height, Border border);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `height` | `unsigned int` | The height of the line in rows. |
| `border` | `Border` | The border style for the line. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the vertical line to a terminal-ready string. |
| `height()` | `unsigned int` | Returns the current height. |

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
| `render()` | `std::string` | Renders the progress bar to a terminal-ready string. |
| `width()` | `unsigned int` | Returns the current width. |

### Notes

- The `decimal` parameter should be between 0.0 (empty) and 1.0 (full).
- Use `Colors::` namespace for predefined colors or create custom colors.
- The progress bar fills from left to right based on the `decimal` value.

---
