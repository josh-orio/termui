# termui — Interface Classes

Interface classes in termui are modal UI components that present a single screen of interaction and wait for user input before returning control to the main application. They inherit from `termui::Interface` and implement display, input processing, and size management.

---

## `termui::Interface`

The base class for all modal interfaces. Provides common functionality for terminal setup, state management, and the core interface lifecycle.

### Enum: `State`

```cpp
enum class State { EXIT, CONTINUE, SELECT };
```

| Value | Description |
|-------|-------------|
| `EXIT` | Interface should close and return control to the main application. |
| `CONTINUE` | Interface should close and continue execution. |
| `SELECT` | Interface closed with a selection made. |

### Constructors

```cpp
Interface();
Interface(TermSetup ts);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `ts` | `TermSetup` | Terminal setup configuration. |

### Protected Members

| Member | Type | Description |
|--------|------|-------------|
| `state` | `State` | Current interface state. |
| `term_setup` | `TermSetup` | Terminal setup configuration. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `display()` | `void` | Renders the interface to the terminal. Virtual; can be overridden. |
| `process_input()` | `void` | Processes user input and updates state. Virtual; can be overridden. |
| `update_size()` | `void` | Updates the interface dimensions based on terminal size. Virtual; can be overridden. |

### Notes

- All interface classes inherit from `Interface` and must implement `display()`, `process_input()`, and `update_size()`.
- The `reprint` flag indicates whether the interface should be redrawn after modifications.
- `display()` is responsible for rendering the entire interface to the terminal.
- `process_input()` handles keyboard input and updates internal state accordingly.
- `update_size()` recalculates dimensions when the terminal size changes.

---

## `termui::BinaryMenu`

Presents a simple yes/no (affirmative/negative) dialog with a title and descriptive text.

### Constructors

```cpp
BinaryMenu(const termui::string &title, const termui::string &text, const termui::string &affirmative, const termui::string &negative);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The dialog title. |
| `text` | `termui::string` | Descriptive text displayed in the dialog. |
| `affirmative` | `termui::string` | Text displayed for the affirmative (yes) option. |
| `negative` | `termui::string` | Text displayed for the negative (no) option. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `bool` | Shows the binary menu and waits for user input. Returns `true` for affirmative, `false` for negative. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title`, `text`, `affirmative`, `negative` | `termui::string` | Dialog content strings. |
| `title_label`, `body` | `Text` | Text rendering components. |
| `aff`, `neg` | `Button` | Buttons for affirmative and negative options. |
| `box` | `Box` | Layout container. |
| `w`, `h` | `size_t` | Width and height of the dialog. |
| `aff_x1`, `aff_x2`, `aff_y` | `size_t` | Bounds for detecting affirmative button press. |
| `neg_x1`, `neg_x2`, `neg_y` | `size_t` | Bounds for detecting negative button press. |
| `selection` | `bool` | Current selection state (true = affirmative). |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Button` widgets for affirmative and negative options.
- Button positions are tracked to detect mouse/keyboard presses.
- The `display()` method renders the title, text, and buttons.
- The `process_input()` method handles key presses and updates the `selection` state.
- Returns `true` if affirmative was selected, `false` otherwise.

---

## `termui::FancyMenu`

Presents a menu with titles and descriptions, featuring enhanced visual styling with borders and separators.

### Constructors

```cpp
FancyMenu(const termui::string &title, const std::vector<FancyList::Element> &rows, uint line_seperation = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The menu title. |
| `rows` | `std::vector<FancyList::Element>` | Vector of menu elements with titles and descriptions. |
| `line_seperation` | `uint` | Number of blank lines between elements. Defaults to `1`. |

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
| `show()` | `termui::Interface::State` | Shows the fancy menu and waits for user input. Returns `EXIT` to close, `SELECT` with selection made. |
| `cursor()` | `uint` | Returns the current cursor position (zero-indexed element index). |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Menu title. |
| `text`, `desc` | `termui::strings` | Menu text and description vectors. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `list` | `FancyList` | The fancy list widget. |
| `line_seperation` | `uint` | Blank lines between elements. |
| `lvo`, `lho` | `uint` | List vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `FancyList` widget for rendering menu items with titles and descriptions.
- The `display()` method renders the title banner and the fancy list.
- The `process_input()` method handles cursor navigation and selection.
- Returns `EXIT` if the user pressed ESC or similar to close, `SELECT` otherwise.

---

## `termui::InfoBox`

Presents an informational dialog with a title, content, and a close button.

### Constructors

```cpp
InfoBox(const termui::string &t, const termui::string &c);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `t` | `termui::string` | The dialog title. |
| `c` | `termui::string` | The dialog content. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the info box and waits for the close button to be pressed. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title`, `content` | `termui::string` | Dialog title and content. |
| `box` | `Box` | Layout container. |
| `header`, `body` | `Text` | Text rendering components. |
| `close` | `Button` | Close button. |
| `w`, `h` | `uint` | Width and height of the dialog. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- The info box displays until the close button is pressed.
- Uses `Button` widget for the close functionality.
- The `display()` method renders the header, content, and close button.
- The `process_input()` method handles button presses and updates the `reprint` flag.

---

## `termui::InfoPage`

Presents paginated informational content with navigation controls.

### Constructors

```cpp
InfoPage(const termui::string &t, const termui::string &c);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `t` | `termui::string` | The page title. |
| `c` | `termui::string` | The paginated content. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the info page and waits for user input. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title`, `content` | `termui::string` | Page title and content. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `info` | `Pager` | Pager widget for scrolling content. |
| `voh`, `hoh` | `uint` | Vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Methods (Overridden)

| Method | Returns | Description |
|--------|---------|-------------|
| `cursor_up(uint count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(uint count = 1)` | `void` | Moves the cursor down by `count` lines. |

### Notes

- Uses `Pager` widget for scrolling through paginated content.
- The `display()` method renders the title banner and pager.
- The `process_input()` method handles cursor navigation and page updates.
- `cursor_up` and `cursor_down` default to moving one line at a time if `count` is omitted.
- The `voh` (vertical overhead) accounts for header and footer display.
- The `hoh` (horizontal overhead) accounts for left and right margins.

---

## `termui::InputBox`

Presents a single input field with a header and placeholder support.

### Constructors

```cpp
InputBox(const termui::string &field, const termui::string &response, const termui::string &placeholder);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `field` | `termui::string` | The input field label. |
| `response` | `termui::string` | The response text to display. |
| `placeholder` | `termui::string` | Placeholder text shown when the field is empty. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the input box and waits for user input. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `field`, `response`, `placeholder` | `termui::string` | Input field label, response, and placeholder. |
| `header` | `Text` | Header rendering component. |
| `input_field` | `Input` | Input widget. |
| `box` | `Box` | Layout container. |
| `w`, `h` | `uint` | Width and height of the input box. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Input` widget for text entry with placeholder support.
- The `display()` method renders the header and input field.
- The `process_input()` method handles text input and updates the `reprint` flag.
- The `placeholder` is displayed when the input field is empty.

---

## `termui::InputPage`

Presents paginated input fields with navigation controls.

### Constructors

```cpp
InputPage(const termui::string &title, const termui::strings &fields, const termui::strings &responses, uint ls = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The page title. |
| `fields` | `termui::strings` | Vector of input field labels. |
| `responses` | `termui::strings` | Vector of response texts. |
| `ls` | `uint` | Line spacing multiplier. Defaults to `0`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the input page and waits for user input. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Page title. |
| `fields`, `responses` | `termui::strings` | Field labels and response texts. |
| `header` | `Text` | Header rendering component. |
| `cursor` | `uint` | Index of the currently selected row. |
| `selected` | `bool` | Whether the current row is selected/active. |
| `visible_lines` | `uint` | Total rows in view. |
| `start_line` | `uint` | Index of the first visible row. |
| `line_seperation` | `uint` | Blank rows between elements. |
| `voh`, `hoh` | `uint` | Vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Methods (Overridden)

| Method | Returns | Description |
|--------|---------|-------------|
| `cursor_up(uint count = 1)` | `void` | Moves the cursor up by `count` lines. |
| `cursor_down(uint count = 1)` | `void` | Moves the cursor down by `count` lines. |

### Notes

- Supports multiple input fields displayed in a paginated layout.
- The `display()` method renders the header and input fields.
- The `process_input()` method handles cursor navigation and field updates.
- `cursor_up` and `cursor_down` default to moving one row at a time if `count` is omitted.
- The `voh` (vertical overhead) accounts for header and footer display.
- The `hoh` (horizontal overhead) accounts for left and right margins.

---

## `termui::Menu`

Presents a simple scrollable menu of text elements with cursor navigation.

### Constructors

```cpp
Menu(const termui::string &title, const termui::strings &elements, uint line_seperation = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The menu title. |
| `elements` | `termui::strings` | Vector of menu elements. |
| `line_seperation` | `uint` | Number of blank lines between elements. Defaults to `1`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `int` | Shows the menu and waits for user input. Returns `-1` for exit, or the index of the selected element. |
| `cursor()` | `uint` | Returns the current cursor position. |
| `status()` | `termui::Interface::State` | Returns the current interface state when closing. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Menu title. |
| `elements` | `termui::strings` | Menu elements. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `list` | `List` | List widget. |
| `line_seperation` | `uint` | Blank lines between elements. |
| `lvo`, `lho` | `uint` | List vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `List` widget for rendering menu items.
- The `display()` method renders the title banner and list.
- The `process_input()` method handles cursor navigation and selection.
- Returns `-1` if the user pressed ESC or similar to exit, or the index of the selected element otherwise.

---

## `termui::Spreadsheet`

Presents a scrollable table for viewing and editing spreadsheet-like data.

### Constructors

```cpp
Spreadsheet(const termui::string &title, const termui::strings &cols, const std::vector<termui::strings> &data);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The spreadsheet title. |
| `cols` | `termui::strings` | Column headers. |
| `data` | `std::vector<termui::strings>` | Row data. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the spreadsheet and waits for user input. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Spreadsheet title. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `table` | `Table` | Table widget. |
| `voh`, `hoh` | `int` | Vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Table` widget for rendering the spreadsheet data.
- The `display()` method renders the title banner and table.
- The `process_input()` method handles table navigation and cell editing.
- The `voh` (vertical overhead) accounts for header display.
- The `hoh` (horizontal overhead) accounts for margins.

---

## `termui::TextEditor`

Presents a paginated text editor with line numbers for editing large text blocks.

### Constructors

```cpp
TextEditor(const termui::string &title, const termui::string &content);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The editor title. |
| `content` | `termui::string` | The text content to edit. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the text editor and waits for user input. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title`, `content` | `termui::string` | Editor title and content. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `pager` | `Pager` | Pager widget for scrolling content. |
| `toh`, `voh`, `hoh` | `int` | Title, vertical, and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Pager` widget for scrolling through the text content.
- The `display()` method renders the title banner and pager.
- The `process_input()` method handles text editing and cursor navigation.
- The `toh` (title overhead) accounts for left/right margins.
- The `voh` (vertical overhead) accounts for header and footer display.
- The `hoh` (horizontal overhead) accounts for line numbers and right margin.

---

## `termui::ToggleMenu`

Presents a menu with toggleable options, allowing multiple selections.

### Constructors

```cpp
ToggleMenu(const termui::string &title, const termui::strings &elements, uint line_seperation = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The menu title. |
| `elements` | `termui::strings` | Vector of menu elements. |
| `line_seperation` | `uint` | Number of blank lines between elements. Defaults to `1`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the toggle menu and waits for user input. |
| `cursor()` | `uint` | Returns the current cursor position. |
| `isSelected(int i)` | `bool` | Returns whether the element at index `i` is selected. |
| `selmap()` | `std::vector<uint>` | Returns a vector of indices of all selected elements. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Menu title. |
| `elements` | `termui::strings` | Menu elements. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `list` | `ToggleList` | Toggle list widget. |
| `line_seperation` | `uint` | Blank lines between elements. |
| `lvo`, `lho` | `uint` | List vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `ToggleList` widget for rendering toggleable menu items.
- Each element can be independently selected or deselected.
- The `display()` method renders the title banner and toggle list.
- The `process_input()` method handles cursor navigation and toggle operations.
- `isSelected(i)` checks the selection state of a specific element.
- `selmap()` returns all indices of currently selected elements.

---
