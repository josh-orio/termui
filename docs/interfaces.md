# termui — Interface Classes

Interface classes in termui are modal UI components that present a single screen of interaction and wait for user input before returning control to the main application. They inherit from `termui::Interface`, and each one sets up the terminal, runs its own input loop until it closes, and restores the terminal.

---

## `termui::Interface`

The base class for all modal interfaces. Provides common functionality for terminal setup, state management, and the core interface lifecycle.

### Enum: `State`

```cpp
enum class State { Exit, Continue, Select, Submit, Delete };
```

| Value | Description |
|-------|-------------|
| `Exit` | The user closed the interface, usually with ESC. |
| `Continue` | The interface is still open and its input loop keeps running. |
| `Select` | The user made a selection. |
| `Submit` | Reserved; not currently used by any interface. |
| `Delete` | Reserved; not currently used by any interface. |

### Constructors

```cpp
Interface();
Interface(TermSetup ts);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `ts` | `TermSetup` | Terminal setup configuration. |

Both constructors are `protected`, so `Interface` is only used as a base class.

### Protected Members

| Member | Type | Description |
|--------|------|-------------|
| `state` | `State` | Current interface state. |
| `term_setup` | `TermSetup` | Terminal setup configuration. |

### Notes

- `Interface` declares no virtual methods. Each subclass provides its own public `show()`, and by convention private `display()`, `process_input()` and `update_size()` methods.
- `show()` calls `term_setup.configure()`, runs the input loop, then calls `term_setup.reset()`.
- The `reprint` flag indicates whether the interface should be redrawn after modifications.
- `display()` is responsible for rendering the entire interface to the terminal.
- `process_input()` handles keyboard input and updates internal state accordingly.
- `update_size()` recalculates dimensions from the current terminal size.
- Every interface uses `TermSetups::fullscreen`, except `InputBox` which uses `TermSetups::fullscreen_with_cursor`.

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
| `control_strip` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `w`, `h` | `size_t` | Width and height of the dialog. |
| `aff_x1`, `aff_x2`, `aff_y` | `size_t` | Bounds for detecting affirmative button press. |
| `neg_x1`, `neg_x2`, `neg_y` | `size_t` | Bounds for detecting negative button press. |
| `selection` | `bool` | Current selection state (true = affirmative). |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Button` widgets for affirmative and negative options.
- Button positions are tracked to detect mouse clicks.
- The `display()` method renders the title, text, and buttons.
- Left and right arrows move the selection, and Enter or a left click confirms it. ESC doesn't close the dialog.
- Returns `true` if affirmative was selected, `false` otherwise.

---

## `termui::FancyMenu`

Presents a menu with titles and descriptions, featuring enhanced visual styling with borders and separators.

### Constructors

```cpp
FancyMenu(const termui::string &title, const std::vector<FancyList::Element> &rows, unsigned int line_seperation = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The menu title. |
| `rows` | `std::vector<FancyList::Element>` | Vector of menu elements with titles and descriptions. |
| `line_seperation` | `unsigned int` | Number of blank lines between elements. Defaults to `1`. |

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
| `show()` | `termui::Interface::State` | Shows the fancy menu and waits for user input. Returns `Exit` if closed, or `Select` if a selection was made. |
| `cursor()` | `unsigned int` | Returns the current cursor position (zero-indexed element index). |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Menu title. |
| `text`, `desc` | `termui::strings` | Menu text and description vectors. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `list` | `FancyList` | The fancy list widget. |
| `control_banner` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `line_seperation` | `unsigned int` | Blank lines between elements. |
| `lvo`, `lho` | `unsigned int` | List vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `FancyList` widget for rendering menu items with titles and descriptions.
- The `display()` method renders the title banner and the fancy list.
- The `process_input()` method handles cursor navigation and selection.
- Up/down arrows and the scroll wheel move the cursor, Enter selects, and ESC closes.
- Returns `Exit` if the user pressed ESC, or `Select` if they pressed Enter.

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
| `show()` | `void` | Shows the info box and waits for the user to close it. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title`, `content` | `termui::string` | Dialog title and content. |
| `box` | `Box` | Layout container. |
| `header`, `body` | `Text` | Text rendering components. |
| `close` | `Button` | Close button. |
| `control_banner` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `w`, `h` | `unsigned int` | Width and height of the dialog. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- The info box displays until the user presses Enter or ESC.
- The `Button` widget draws the close button; it doesn't respond to mouse clicks.
- The `display()` method renders the header, content, and close button.
- The `process_input()` method handles the Enter and ESC keys.

---

## `termui::InfoPage`

Presents scrollable informational content with navigation controls.

### Constructors

```cpp
InfoPage(const termui::string &t, const termui::string &c);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `t` | `termui::string` | The page title. |
| `c` | `termui::string` | The content to scroll through. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the info page and waits for user input. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title`, `content` | `termui::string` | Page title and content. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `info` | `Pager` | Pager for scrolling content. |
| `control_strip` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `voh`, `hoh` | `unsigned int` | Vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Pager` for scrolling through the content.
- The `display()` method renders the title banner and pager.
- Up/down arrows and the scroll wheel scroll one line, Shift + up/down scrolls five lines, and ESC closes.
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
| `response` | `termui::string` | The response text. Edited in place, so the caller's string holds the result after `show()` returns. |
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
| `input_field` | `Input` | Input field. |
| `control_strip` | `static Text` | Footer showing the controls. |
| `box` | `Box` | Layout container. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `w`, `h` | `unsigned int` | Width and height of the input box. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Input` for text entry with placeholder support.
- The `display()` method renders the header and input field.
- Printable characters are appended, Backspace removes the last character, and ESC closes. Enter does nothing.
- The `placeholder` is displayed when the input field is empty.

---

## `termui::InputPage`

Presents a scrollable page of input fields with navigation controls.

### Constructors

```cpp
InputPage(const termui::string &title, const termui::strings &fields, const termui::strings &responses, unsigned int ls = 0);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The page title. |
| `fields` | `termui::strings` | Vector of input field labels. |
| `responses` | `termui::strings` | Vector of response texts. Edited in place, so the caller's strings hold the results after `show()` returns. |
| `ls` | `unsigned int` | Line spacing multiplier. Defaults to `0`. |

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
| `control_strip` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `cursor` | `unsigned int` | Index of the currently selected row. |
| `selected` | `bool` | Whether the current row is selected/active. |
| `visible_lines` | `unsigned int` | Total rows in view. |
| `start_line` | `unsigned int` | Index of the first visible row. |
| `line_seperation` | `unsigned int` | Blank rows between elements. |
| `voh`, `hoh` | `unsigned int` | Vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Supports multiple input fields displayed in a scrolling layout.
- The `display()` method renders the header and input fields.
- Up/down arrows and the scroll wheel move between fields. Enter starts or stops editing the current field.
- While editing, printable characters are appended and Backspace removes the last character.
- ESC closes the page, but only when no field is being edited.
- The `voh` (vertical overhead) accounts for header and footer display.
- The `hoh` (horizontal overhead) accounts for left and right margins.

---

## `termui::Menu`

Presents a simple scrollable menu of text elements with cursor navigation.

### Constructors

```cpp
Menu(const termui::string &title, const termui::strings &elements, unsigned int line_seperation = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The menu title. |
| `elements` | `termui::strings` | Vector of menu elements. |
| `line_seperation` | `unsigned int` | Number of blank lines between elements. Defaults to `1`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `int` | Shows the menu and waits for user input. Returns `-1` for exit, or the index of the selected element. |
| `cursor()` | `unsigned int` | Returns the current cursor position. |
| `status()` | `termui::Interface::State` | Returns the state the interface closed with: `Exit` or `Select`. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Menu title. |
| `elements` | `termui::strings` | Menu elements. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `list` | `List` | List widget. |
| `control_banner` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `line_seperation` | `unsigned int` | Blank lines between elements. |
| `lvo`, `lho` | `unsigned int` | List vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `List` widget for rendering menu items.
- The `display()` method renders the title banner and list.
- Up/down arrows and the scroll wheel move the cursor, Shift + up/down moves five elements, Enter selects, and ESC closes.
- Returns `-1` if the user pressed ESC, or the index of the selected element otherwise.

---

## `termui::Spreadsheet`

Presents a scrollable table for viewing spreadsheet-like data.

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
| `control_strip` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `voh`, `hoh` | `int` | Vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Table` widget for rendering the spreadsheet data.
- The `display()` method renders the title banner and table.
- Up/down arrows move between rows and ESC closes. Cells can't be edited.
- The `voh` (vertical overhead) accounts for header display.
- The `hoh` (horizontal overhead) accounts for margins.

---

## `termui::TextEditor`

Presents a scrolling text editor with line numbers for editing large text blocks.

### Constructors

```cpp
TextEditor(const termui::string &title, const termui::string &content);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The editor title. |
| `content` | `termui::string` | The text content to edit. Edited in place, so the caller's string holds the result after `show()` returns. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the text editor and waits for user input. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title`, `content` | `termui::string` | Editor title and content. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `pager` | `Pager` | Pager for scrolling content. |
| `control_strip` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `toh`, `voh`, `hoh` | `int` | Title, vertical, and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `Pager` for scrolling through the text content.
- The `display()` method renders the title banner and pager.
- Editing only happens at the end of the content: printable characters and Enter append, and Backspace removes the last character. The left and right arrows do nothing.
- Up/down arrows and the scroll wheel scroll, and ESC closes.
- The `toh` (title overhead) accounts for left/right margins.
- The `voh` (vertical overhead) accounts for header and footer display.
- The `hoh` (horizontal overhead) accounts for line numbers and right margin.

---

## `termui::ToggleMenu`

Presents a menu with toggleable options, allowing multiple selections.

### Constructors

```cpp
ToggleMenu(const termui::string &title, const termui::strings &elements, unsigned int line_seperation = 1);
```

| Parameter | Type | Description |
|-----------|------|-------------|
| `title` | `termui::string` | The menu title. |
| `elements` | `termui::strings` | Vector of menu elements. |
| `line_seperation` | `unsigned int` | Number of blank lines between elements. Defaults to `1`. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `show()` | `void` | Shows the toggle menu and waits for user input. |
| `cursor()` | `unsigned int` | Returns the current cursor position. |
| `isSelected(int i)` | `bool` | Returns whether the element at index `i` is selected. |

### Private Members

| Member | Type | Description |
|--------|------|-------------|
| `title` | `termui::string` | Menu title. |
| `elements` | `termui::strings` | Menu elements. |
| `title_banner` | `PaddedText` | Title rendering component. |
| `list` | `ToggleList` | Toggle list widget. |
| `control_banner` | `static Text` | Footer showing the controls. |
| `styles` | `static StyleMap` | Styles used by the interface. |
| `line_seperation` | `unsigned int` | Blank lines between elements. |
| `lvo`, `lho` | `unsigned int` | List vertical and horizontal overhead. |
| `reprint` | `bool` | Flag indicating if reprint is required. |

### Notes

- Uses `ToggleList` widget for rendering toggleable menu items.
- Each element can be independently selected or deselected.
- The `display()` method renders the title banner and toggle list.
- Up/down arrows and the scroll wheel move the cursor, Enter toggles the current element, and ESC closes.
- `isSelected(i)` checks the selection state of a specific element.
- `selmap()` is declared in the header but has no implementation, so use `isSelected(i)` instead.

---
