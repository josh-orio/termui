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
