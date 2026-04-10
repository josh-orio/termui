# termui — Widget Classes

Widget classes in termui are interactive UI components that combine text rendering with user input handling. They extend base classes to provide specialized functionality for common TUI patterns.

---

## `termui::Button`

Renders a clickable button with separate styles for focused and unfocused states. Supports keyboard navigation and visual feedback.

### Constructors

```cpp
Button(const termui::string &text, Style focus_style = Styles::none, Style blur_style = Styles::none, uint width = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `text` | `termui::string` | The button label text to display. |
| `focus_style` | `Style` | Style applied when the button is focused (keyboard selected). Defaults to `Styles::none`. |
| `blur_style` | `Style` | Style applied when the button is unfocused. Defaults to `Styles::none`. |
| `width` | `uint` | Constraining width in columns. `0` means unconstrained. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the button to a terminal-ready string. |
| `focus()` | `void` | Sets the button as the currently focused element. |
| `blur()` | `void` | Removes focus from the button. |
| `resize(uint w)` | `void` | Resizes the button to the specified width. |
| `width()` | `uint` | Returns the current width of the button. |

### Notes

- `focus()` and `blur()` modify the `selected` state internally.
- The `render()` method checks the `selected` state to apply the appropriate style.
- `blur_style` is used when the button is not selected/focused.

---

## `termui::FancyList`

Renders a scrollable list of elements with titles and descriptions, featuring enhanced visual styling with borders and separators.

### Constructors

```cpp
FancyList(const std::vector<Element> &rows, Style focus_style = Styles::none, Style blur_style = Styles::none, uint width = 0, uint height = 0, uint ls = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `rows` | `std::vector<Element>` | Vector of list elements, each containing a title and description. |
| `focus_style` | `Style` | Style applied to the focused element. Defaults to `Styles::none`. |
| `blur_style` | `Style` | Style applied to unfocused elements. Defaults to `Styles::none`. |
| `width` | `uint` | Constraining width in columns. `0` means unconstrained. |
| `height` | `uint` | Constraining height in rows. `0` means unconstrained. |
| `ls` | `uint` | Line spacing multiplier. Defaults to `1`. |

### Element Structure

```cpp
struct Element {
  termui::string title, description;

  Element(const termui::string &title, const termui::string &description) : title(title), description(description) {}
};
```

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | The display title for the element. |
| `description` | `termui::string` | The description text for the element. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the fancy list to a terminal-ready string. |
| `resize(uint w, uint h)` | `void` | Resizes the list to the specified width and height. |
| `resize(uint w, uint h, uint ls)` | `void` | Resizes the list with custom line spacing. |
| `cursor_up(uint count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(uint count = 1)` | `void` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `uint` | Returns the current cursor position (zero-indexed element index). |

### Notes

- The cursor represents the currently selected/focused element.
- `cursor_up` and `cursor_down` default to moving one element at a time if `count` is omitted.
- Line spacing (`ls`) controls the vertical space between elements.
- The `internal_update()` method recalculates visible rows and cursor position after modifications.

---

## `termui::List`

Renders a simple scrollable list of text elements with cursor navigation.

### Constructors

```cpp
List(const termui::strings &strs, Style focus_style = Styles::none, Style blur_style = Styles::none, uint width = 0, uint height = 0, uint ls = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `strs` | `termui::strings` | Vector of string elements to display. |
| `focus_style` | `Style` | Style applied to the focused element. Defaults to `Styles::none`. |
| `blur_style` | `Style` | Style applied to unfocused elements. Defaults to `Styles::none`. |
| `width` | `uint` | Constraining width in columns. `0` means unconstrained. |
| `height` | `uint` | Constraining height in rows. `0` means unconstrained. |
| `ls` | `uint` | Line spacing multiplier. Defaults to `0`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the list to a terminal-ready string. |
| `resize(uint w, uint h)` | `void` | Resizes the list to the specified width and height. |
| `cursor_up(uint count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(uint count = 1)` | `void` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `uint` | Returns the current cursor position (zero-indexed element index). |

### Notes

- The cursor represents the currently selected/focused element.
- `cursor_up` and `cursor_down` default to moving one element at a time if `count` is omitted.
- Line spacing (`ls`) controls the vertical space between elements.
- The `internal_update()` method recalculates visible lines and cursor position after modifications.

---

## `termui::Table`

Renders a scrollable table with configurable columns, rows, and styling options including borders and cursor highlighting.

### Constructors

```cpp
Table(const std::vector<Column> &columns, const std::vector<Row> &rows, const TableStyle &ts);
Table(const termui::strings &columns, const std::vector<termui::strings> &rows, const TableStyle &ts);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `columns` | `std::vector<Column>` or `termui::strings` | Column definitions or header strings. |
| `rows` | `std::vector<Row>` or `std::vector<termui::strings>` | Row data (cells or strings). |
| `ts` | `TableStyle` | Table styling configuration. |

### Column Structure

```cpp
struct Column {
  termui::string title;
  uint width;

  Column(termui::string str, uint w = 1);
};
```

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | The column header text. |
| `width` | `uint` | The width of the column in characters. Defaults to `1`. |

### Row Structure

```cpp
struct Row {
  termui::strings cells;

  Row(termui::strings c);
};
```

| Member | Type | Description |
|--------|------|-------------|
| `cells` | `termui::strings` | Vector of cell strings in the row. |

### TableStyle Structure

```cpp
struct TableStyle {
  uint table_height;
  uint cell_height = 1;
  uint line_seperation = 0;
  Style cursor_style = Style(std::nullopt, 57);
  Border border = Borders::rounded;
};
```

| Member | Type | Description |
|--------|------|-------------|
| `table_height` | `uint` | The height of the table in rows. |
| `cell_height` | `uint` | Height of individual cells. Defaults to `1`. |
| `line_seperation` | `uint` | Additional spacing between rows. Defaults to `0`. |
| `cursor_style` | `Style` | Style applied to the cursor/focused cell. Defaults to ANSI yellow. |
| `border` | `Border` | Border style for the table. Defaults to `Borders::rounded`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the table to a terminal-ready string. |
| `resize(std::vector<uint> widths, uint h)` | `void` | Resizes the table with custom column widths and height. |
| `cursor_up(uint count = 1)` | `void` | Moves the cursor up by `count` rows. |
| `cursor_down(uint count = 1)` | `void` | Moves the cursor down by `count` rows. |
| `get_cursor()` | `uint` | Returns the current cursor position (zero-indexed row index). |
| `colCount()` | `uint` | Returns the number of columns in the table. |

### Notes

- The cursor represents the currently focused cell (row index).
- `cursor_up` and `cursor_down` default to moving one row at a time if `count` is omitted.
- The table supports both structured column/row definitions and simple string-based construction.
- The `internal_update()` method recalculates visible rows and cursor position after modifications.
- The `overhead` parameter reserves lines for header and footer display.

---

## `termui::ToggleList`

Renders a scrollable list of elements where each element can be individually toggled on/off.

### Constructors

```cpp
ToggleList(const termui::strings &strs, Style focus_style = Styles::none, Style blur_style = Styles::none, uint width = 0, uint height = 0, uint ls = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `strs` | `termui::strings` | Vector of string elements to display. |
| `focus_style` | `Style` | Style applied to the focused element. Defaults to `Styles::none`. |
| `blur_style` | `Style` | Style applied to unfocused elements. Defaults to `Styles::none`. |
| `width` | `uint` | Constraining width in columns. `0` means unconstrained. |
| `height` | `uint` | Constraining height in rows. `0` means unconstrained. |
| `ls` | `uint` | Line spacing multiplier. Defaults to `0`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `std::string` | Renders the toggle list to a terminal-ready string. |
| `resize(uint w, uint h)` | `void` | Resizes the list to the specified width and height. |
| `cursor_up(uint count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(uint count = 1)` | `void` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `uint` | Returns the current cursor position (zero-indexed element index). |
| `toggle()` | `void` | Toggles the selection state of the currently focused element. |
| `getSelection(int i)` | `bool` | Returns the selection state of element at index `i`. |

### Notes

- Each element has an independent selection state tracked in `selmap`.
- The cursor represents the currently focused element.
- `toggle()` flips the selection state of the element at the current cursor position.
- `getSelection(i)` returns `true` if the element at index `i` is selected, `false` otherwise.
- `cursor_up` and `cursor_down` default to moving one element at a time if `count` is omitted.
- Line spacing (`ls`) controls the vertical space between elements.
- The `internal_update()` method recalculates visible lines and cursor position after modifications.

---
