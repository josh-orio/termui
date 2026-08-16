# termui — Style & Color

`Color` and `Style` describe how text should be rendered in the terminal: foreground/background colour and SGR text attributes (bold, underline, etc). Both types have an explicit notion of **inherit** (leave whatever was already set alone) as distinct from an explicit **reset** (clear back to terminal defaults), which lets styles be layered and composed safely.

---

## `termui::Color`

Represents a terminal colour, or a special non-colour state (`Inherit` / `Reset`).

### Modes

```cpp
enum class Mode { INHERIT, RESET, ANSI256, TRUECOLOR };
```

| Mode | Meaning |
|------|---------|
| `INHERIT` | No colour is set; whatever colour is already active is left untouched. |
| `RESET` | Explicitly reset to the terminal's default colour. |
| `ANSI256` | An indexed 256-colour palette value. |
| `TRUECOLOR` | A 24-bit RGB colour. |

### Constructors

```cpp
Color();
Color(uint8_t v);
Color(uint8_t r, uint8_t g, uint8_t b);
```

| Constructor | Mode | Description |
|-------------|------|-------------|
| `Color()` | `INHERIT` | Default-constructing a `Color` means "don't touch the existing colour." Use this when you don't want a `Style` to affect the foreground or background at all. |
| `Color(uint8_t v)` | `ANSI256` | A colour from the 256-colour palette, identified by index `v`. |
| `Color(uint8_t r, uint8_t g, uint8_t b)` | `TRUECOLOR` | A 24-bit RGB colour. |

### Static factories

| Method | Returns | Description |
|--------|---------|-------------|
| `Color::Reset()` | `Color` | A colour that explicitly resets to the terminal's default, rather than leaving the current colour untouched. |
| `Color::Inherit()` | `Color` | Equivalent to the default constructor — explicitly requests "don't touch the existing colour." |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `operator==` | `bool` | Compares mode and value/RGB for equality. |
| `IsInherit()` | `bool` | `true` if this colour is in `INHERIT` mode. |
| `IsReset()` | `bool` | `true` if this colour is in `RESET` mode. |

### Notes

- Internally, `value` (for `ANSI256`) and `rgb` (for `TRUECOLOR`) share a `union`; only the member matching the current `mode` is meaningful.
- `INHERIT` and `RESET` are distinct: inheriting leaves the current colour as-is (useful when composing styles), while reset actively clears it.

---

## `termui::SGR`

The set of terminal text attributes ("Select Graphic Rendition") that a `Style` can apply, independently of colour.

```cpp
enum class SGR : uint8_t {
  Inherit    = 255,
  None       = 0,
  Bold       = 1,
  Faint      = 2,
  Italic     = 3,
  Underlined = 4,
  Blink      = 5,
  FastBlink  = 6,
  ReverseVideo = 7,
  Conceal    = 8,
  CrossedOut = 9,
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
| `CrossedOut` | Strikethrough text. |

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
| `Style()` | An empty style: foreground and background both inherit, no attributes. Applying it has no visible effect. |
| `Style(fg, bg)` | Sets foreground and background colour only; no attributes are touched. |
| `Style(fg, bg, sgr)` | Sets foreground, background, and a list of attributes. |
| `Style(sgr)` | Sets attributes only; foreground and background both inherit. |

### Methods

| Method | Returns | Description |
|--------|---------|-------------|
| `Foreground()` | `const Color&` | The style's foreground colour. |
| `Background()` | `const Color&` | The style's background colour. |
| `Attributes()` | `const std::vector<SGR>&` | The style's list of `SGR` attributes. |
| `operator==` / `operator!=` | `bool` | Compares foreground, background, and attributes for equality. |
| `InheritsSGR()` | `bool` | `true` only if `Attributes()` is exactly `{ SGR::Inherit }` — i.e. this style explicitly leaves existing attributes untouched. |
| `ResetsSGR()` | `bool` | `true` only if `Attributes()` is exactly `{ SGR::None }` — i.e. this style explicitly clears all attributes. |

### Notes

- Like `Color`, a default-constructed `Style` is inert: it inherits foreground, background, and attributes, so applying it changes nothing.
- `InheritsSGR()` and `ResetsSGR()` only return `true` for the specific single-element sentinel vectors described above. A style with a mix of real attributes (e.g. `{ Bold, Underlined }`) returns `false` for both.
- `Foreground()`, `Background()`, and `Attributes()` return references into the `Style`; they're valid as long as the `Style` itself is alive.
