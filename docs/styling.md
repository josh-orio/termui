# termui — Style & Color

`Color` and `Style` describe how text should be rendered in the terminal: foreground/background colour and SGR text attributes (bold, underline, etc). Both types have an explicit notion of **inherit** (leave whatever was already set alone) as distinct from an explicit **reset** (clear back to terminal defaults), which lets styles be layered and composed safely.

---

## `termui::Color`

Represents a terminal colour, or a special non-colour state (`Inherit` / `Reset`).

### Modes

```cpp
enum class Mode { Inherit, Reset, Ansi256, TrueColor };
```

| Mode | Meaning |
|------|---------|
| `Inherit` | No colour is set; whatever colour is already active is left untouched. |
| `Reset` | Explicitly reset to the terminal's default colour. |
| `Ansi256` | An indexed 256-colour palette value. |
| `TrueColor` | A 24-bit RGB colour. |

### Constructors

```cpp
Color();
Color(uint8_t v);
Color(uint8_t r, uint8_t g, uint8_t b);
Color(std::string hex);
```

| Constructor | Mode | Description |
|-------------|------|-------------|
| `Color()` | `Inherit` | Default-constructing a `Color` means "don't touch the existing colour." Use this when you don't want a `Style` to affect the foreground or background at all. |
| `Color(uint8_t v)` | `Ansi256` | A colour from the 256-colour palette, identified by index `v`. |
| `Color(uint8_t r, uint8_t g, uint8_t b)` | `TrueColor` | A 24-bit RGB colour. |
| `Color(std::string hex)` | `TrueColor` | A 24-bit RGB colour from a hex code in the form `"#abcdef"`. |

### Static factories

| Method | Returns | Description |
|--------|---------|-------------|
| `Color::Reset()` | `Color` | A colour that explicitly resets to the terminal's default, rather than leaving the current colour untouched. |
| `Color::Inherit()` | `Color` | Equivalent to the default constructor — explicitly requests "don't touch the existing colour." |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `operator==` | `bool` | Compares mode and value/RGB for equality. |
| `IsInherit()` | `bool` | `true` if this colour is in `Inherit` mode. |
| `IsReset()` | `bool` | `true` if this colour is in `Reset` mode. |
| `isDark()` | `bool` | `true` if the colour's luminance is below the midpoint. Only meaningful for `TrueColor` colours. |
| `isLight()` | `bool` | The inverse of `isDark()`. |

### Notes

- Internally, `value` (for `Ansi256`) and `rgb` (for `TrueColor`) share a `union`; only the member matching the current `mode` is meaningful.
- `Inherit` and `Reset` are distinct: inheriting leaves the current colour as-is (useful when composing styles), while reset actively clears it.

### Predefined colours

The `termui::Colors` namespace provides `Red`, `Green`, `Yellow`, `Blue`, `Magenta`, `Cyan`, `White`, `Black`, `Lavender`, `Orange` and `Rose`.

---

## `termui::SGR`

The set of terminal text attributes ("Select Graphic Rendition") that a `Style` can apply, independently of colour.

```cpp
enum class SGR : uint8_t {
  Inherit      = 255,
  None         = 0,
  Bold         = 1,
  Faint        = 2,
  Italic       = 3,
  Underlined   = 4,
  Blink        = 5,
  FastBlink    = 6,
  ReverseVideo = 7,
  Conceal      = 8,
  CrossedOut   = 9,
};
```

| Value | Description |
|-------|-------------|
| `Inherit` | Sentinel used by `Style` to mean "don't touch existing attributes." Not a real terminal attribute. |
| `None` | Sentinel used by `Style` to mean "reset all attributes." Not a real terminal attribute. |
| `Bold` | Bold/increased intensity text. |
| `Faint` | Faint/decreased intensity text. |
| `Italic` | Italic text. |
| `Underlined` | Underlined text. |
| `Blink` | Slow blink. |
| `FastBlink` | Fast blink. Support for this varies between terminal emulators. |
| `ReverseVideo` | Swaps foreground and background colours. |
| `Conceal` | Hidden/invisible text. |
| `CrossedOut` | Strikethrough text. Support for this varies between terminal emulators. |

> **Note:** `Inherit` and `None` are sentinel values interpreted specially by `Style` (see `InheritsSGR()` / `ResetsSGR()` below) — don't mix them with real attributes in the same `Style`.

---

## `termui::Style`

Combines a foreground colour, background colour, and a list of `SGR` attributes into a single, composable text style.

### Constructors

```cpp
Style();
Style(Color fg, Color bg);
Style(Color fg, Color bg, std::vector<SGR> sgr);
Style(std::vector<SGR> sgr);
```

| Constructor | Description |
|-------------|-------------|
| `Style()` | An inert style: foreground, background and attributes all inherit. Applying it has no visible effect. |
| `Style(fg, bg)` | Sets foreground and background colour only; attributes inherit. |
| `Style(fg, bg, sgr)` | Sets foreground, background, and a list of attributes. |
| `Style(sgr)` | Sets attributes only; foreground and background both inherit. |

### Members

`Style` is a plain struct, so these are read and written directly.

| Member | Type | Description |
|--------|------|-------------|
| `foregroundColor` | `Color` | The style's foreground colour. |
| `backgroundColor` | `Color` | The style's background colour. |
| `attributes` | `std::vector<SGR>` | The style's list of `SGR` attributes. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `operator==` / `operator!=` | `bool` | Compares foreground, background, and attributes for equality. |
| `InheritsSGR()` | `bool` | `true` only if `attributes` is exactly `{ SGR::Inherit }` — i.e. this style explicitly leaves existing attributes untouched. |
| `ResetsSGR()` | `bool` | `true` only if `attributes` is exactly `{ SGR::None }` — i.e. this style explicitly clears all attributes. |

### Predefined styles

| Style | Description |
|-------|-------------|
| `Styles::none` | Resets colours and attributes, like `\x1b[0m`. |
| `Styles::inherit` | Inherits colours and attributes, so it changes nothing. |
| `Styles::bold` | Bold text; colours inherit. |
| `Styles::faint` | Faint text; colours inherit. |

### Notes

- Like `Color`, a default-constructed `Style` is inert: it inherits foreground, background, and attributes, so applying it changes nothing.
- `InheritsSGR()` and `ResetsSGR()` only return `true` for the specific single-element sentinel vectors described above. A style with a mix of real attributes (e.g. `{ Bold, Underlined }`) returns `false` for both.

---

## `termui::StyleMap`

A collection of named styles, used by the interfaces to look up their styles by key.

### Constructors

```cpp
StyleMap() = default;
StyleMap(std::map<std::string, termui::Style> styles);
```

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `add(std::string key, Style style)` | `void` | Adds a style, replacing any existing style with that key. |
| `at(std::string style)` | `const Style&` | Returns the style with that key. A missing key is added as an inert `Style`. |
| `keys()` | `std::vector<std::string>` | Returns all of the keys, in sorted order. |
