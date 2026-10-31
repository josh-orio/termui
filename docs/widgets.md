# termui — Widget Classes

Widget classes in termui are interactive UI components that combine text rendering with user input handling. They provide specialized functionality for common TUI patterns.

All `render()` methods write through `termui::terminal` into its output buffer. Nothing appears on screen until `terminal.flush()` is called.

> **Note:** Widget sizes are set with chained setters rather than in the constructor, and most of them have no default. Set the values listed in each widget's notes before calling `render()`.

---

## `termui::Button`

Renders a button with separate styles for focused and unfocused states. The text is centred in the button's width, and cut off with `…` if it doesn't fit.

### Constructors

```cpp
Button(const termui::string &text, Style focus_style = {}, Style blur_style = {});
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `text` | `termui::string` | The button label text to display. |
| `focus_style` | `Style` | Style applied when the button is focused. Defaults to an inert `Style`. |
| `blur_style` | `Style` | Style applied when the button is unfocused. Defaults to an inert `Style`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the button to the terminal buffer. |
| `focus_style(Style s)` | `Button&` | Sets the focused style. |
| `blur_style(Style s)` | `Button&` | Sets the unfocused style. |
| `width(unsigned int w)` | `Button&` | Sets the width. |
| `focus()` | `Button&` | Sets the button as focused. |
| `blur()` | `Button&` | Removes focus from the button. |
| `setFocus(bool x)` | `Button&` | Sets the focus state. |

### Notes

- Call `width()` and one of `focus()`, `blur()` or `setFocus()` before rendering.
- `focus()`, `blur()` and `setFocus()` modify the `_selected` state internally.
- The `render()` method checks the `_selected` state to apply the appropriate style.

---

## `termui::FancyList`

Renders a scrollable list of elements with titles and descriptions, featuring enhanced visual styling with borders and separators.

### Constructors

```cpp
FancyList(const std::vector<Element> &rows, Style focus_style = {}, Style blur_style = {});
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `rows` | `std::vector<Element>` | Vector of list elements, each containing a title and description. |
| `focus_style` | `Style` | Style applied to the focused element. Defaults to an inert `Style`. |
| `blur_style` | `Style` | Style applied to unfocused elements. Defaults to an inert `Style`. |

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
| `render()` | `void` | Writes the fancy list to the terminal buffer. |
| `focus_style(Style s)` | `FancyList&` | Sets the focused style. |
| `blur_style(Style s)` | `FancyList&` | Sets the unfocused style. |
| `width(unsigned int w)` | `FancyList&` | Sets the width. |
| `height(unsigned int h)` | `FancyList&` | Sets the height. |
| `line_seperation(unsigned int ls)` | `FancyList&` | Sets the spacing between elements. |
| `cursor_up(unsigned int count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(unsigned int count = 1)` | `void` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `unsigned int` | Returns the current cursor position (zero-indexed element index). |

### Notes

- Call `width()`, `height()` and `line_seperation()` before rendering or moving the cursor.
- The cursor represents the currently selected/focused element.
- `cursor_up` and `cursor_down` default to moving one element at a time if `count` is omitted.
- Line spacing controls the vertical space between elements.
- The private `internal_update()` method recalculates visible rows.

---

## `termui::List`

Renders a simple scrollable list of text elements with cursor navigation.

### Constructors

```cpp
List(const termui::strings &strs, Style focus_style = {}, Style blur_style = {});
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `strs` | `termui::strings` | Vector of string elements to display. |
| `focus_style` | `Style` | Style applied to the focused element. Defaults to an inert `Style`. |
| `blur_style` | `Style` | Style applied to unfocused elements. Defaults to an inert `Style`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the list to the terminal buffer. |
| `focus_style(Style s)` | `List&` | Sets the focused style. |
| `blur_style(Style s)` | `List&` | Sets the unfocused style. |
| `width(unsigned int w)` | `List&` | Sets the width. |
| `height(unsigned int h)` | `List&` | Sets the height. |
| `line_seperation(unsigned int ls)` | `List&` | Sets the spacing between elements. |
| `cursor_up(unsigned int count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(unsigned int count = 1)` | `void` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `unsigned int` | Returns the current cursor position (zero-indexed element index). |

### Notes

- Call `width()`, `height()` and `line_seperation()` before rendering or moving the cursor.
- The cursor represents the currently selected/focused element.
- `cursor_up` and `cursor_down` default to moving one element at a time if `count` is omitted.
- Line spacing controls the vertical space between elements.
- The private `internal_update()` method recalculates visible lines.

---

## `termui::Table`

Renders a scrollable table with configurable columns, rows, and styling options including borders and cursor highlighting.

### Constructors

```cpp
Table(const std::vector<Column> &columns, const std::vector<Row> &rows, const TableStyle &ts = {});
Table(const termui::strings &columns, const std::vector<termui::strings> &rows, const TableStyle &ts = {});
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `columns` | `std::vector<Column>` or `termui::strings` | Column definitions or header strings. |
| `rows` | `std::vector<Row>` or `std::vector<termui::strings>` | Row data (cells or strings). |
| `ts` | `TableStyle` | Table styling configuration. Defaults to `TableStyle()`. |

### Column Structure

```cpp
struct Column {
  termui::string title;
  unsigned int width;

  Column(termui::string str, unsigned int w = 1);
};
```

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | The column header text. |
| `width` | `unsigned int` | The width of the column in characters. Defaults to `1`. |

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
  unsigned int table_height;
  unsigned int cell_height;
  unsigned int line_seperation;
  Style        cursor_style;
  Border       border;

  TableStyle(unsigned int table_height = 0, unsigned int cell_height = 1, unsigned int line_seperation = 0, Style cursor_style = Style(Color::Inherit(), 57),
             Border border = Borders::rounded);
};
```

| Member | Type | Description |
|--------|------|-------------|
| `table_height` | `unsigned int` | The height of the table in rows. Defaults to `0`. |
| `cell_height` | `unsigned int` | Height of individual cells. Defaults to `1`. |
| `line_seperation` | `unsigned int` | Additional spacing between rows. Defaults to `0`. |
| `cursor_style` | `Style` | Style applied to the cursor row. Defaults to a background of palette colour `57`, with the foreground inherited. |
| `border` | `Border` | Border style for the table. Defaults to `Borders::rounded`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the table to the terminal buffer. |
| `column_width(unsigned int col, unsigned int w)` | `Table&` | Sets the width of column `col`. |
| `table_height(unsigned int h)` | `Table&` | Sets the table height. |
| `cell_height(unsigned int h)` | `Table&` | Sets the cell height. |
| `line_seperation(unsigned int ls)` | `Table&` | Sets the spacing between rows. |
| `cursor_up(unsigned int count = 1)` | `void` | Moves the cursor up by `count` rows. |
| `cursor_down(unsigned int count = 1)` | `void` | Moves the cursor down by `count` rows. |
| `get_cursor()` | `unsigned int` | Returns the current cursor position (zero-indexed row index). |
| `colCount()` | `unsigned int` | Returns the number of columns in the table. |

### Notes

- The cursor represents the currently focused row.
- `cursor_up` and `cursor_down` default to moving one row at a time if `count` is omitted.
- The table supports both structured column/row definitions and simple string-based construction.
- The private `internal_update()` method recalculates visible rows.
- The private `_overhead` constant reserves lines for header and footer display.

---

## `termui::ToggleList`

Renders a scrollable list of elements where each element can be individually toggled on/off.

### Constructors

```cpp
ToggleList(const termui::strings &strs, Style focus_style = {}, Style blur_style = {});
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `strs` | `termui::strings` | Vector of string elements to display. |
| `focus_style` | `Style` | Style applied to the focused element. Defaults to an inert `Style`. |
| `blur_style` | `Style` | Style applied to unfocused elements. Defaults to an inert `Style`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `render()` | `void` | Writes the toggle list to the terminal buffer. |
| `focus_style(Style s)` | `ToggleList&` | Sets the focused style. |
| `blur_style(Style s)` | `ToggleList&` | Sets the unfocused style. |
| `width(unsigned int w)` | `ToggleList&` | Sets the width. |
| `height(unsigned int h)` | `ToggleList&` | Sets the height. |
| `line_seperation(unsigned int ls)` | `ToggleList&` | Sets the spacing between elements. |
| `cursor_up(unsigned int count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(unsigned int count = 1)` | `void` | Moves the cursor down by `count` lines. |
| `get_cursor()` | `unsigned int` | Returns the current cursor position (zero-indexed element index). |
| `toggle()` | `void` | Toggles the selection state of the currently focused element. |
| `getSelection(int i)` | `bool` | Returns the selection state of element at index `i`. |

### Notes

- Call `width()`, `height()` and `line_seperation()` before rendering or moving the cursor.
- Each element has an independent selection state tracked in `_selmap`.
- The cursor represents the currently focused element.
- `toggle()` flips the selection state of the element at the current cursor position.
- `getSelection(i)` returns `true` if the element at index `i` is selected, `false` otherwise.
- `cursor_up` and `cursor_down` default to moving one element at a time if `count` is omitted.
- Line spacing controls the vertical space between elements.
- The private `internal_update()` method recalculates visible lines.

---
