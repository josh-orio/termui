# TermUI

Terminal interfaces library for C++.

## Get Started

This library requires nlohmann::json, you can probably get it through a package manager.

Then to build and install the library, ```make``` should do it, no guarantees.

Link the library with your project using:
```cmake
find_package(termui REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE termui::termui)
```

Then you can access all the interfaces by:
```c++
#include <termui/termui.hpp>
```

## Interfaces

This library provides a few types of interfaces so far:

- Menu: select between options speicifed by the program
- Input: program provides fields for a user to give text based input
- Info: program provides scroll capabilities to view large blocks of text
- Table: present table data

## Styling

There are a few ways to customise the appearance of text, all methods are built on ANSI codes and std::string.

Applying color and formatting to text can be done using the raw ANSI code provided in ```formatting.hpp```:
```c++
Console cons ();
cons.print_ln(colors::BLUE + formats::BOLD + "TermUI!" + colors::DEFAULT + formats::NONE);
```

Slightly more advanced and readable is using a ```Style``` object to create reusable themes:
```c++
Console cons ();
Style style (colors::WHITE, backgrounds::RED, formats::BOLD);
cons.print_ln(style.render("TermUI!));
```

For even more flexibility in colors, you can use an ```RGB``` object for full color control:
```c++
Console cons ();
RGB light_blue (102, 204, 255);
Style style (light_blue.fg_ansi(), "", formats::BOLD);
cons.print_ln(style.render("TermUI!));
```
