# TermUI

Terminal interfaces library for C++.

## Get Started

To build and install the library, run ```make install```.

Link the library with your project using:
```cmake
find_package(termui REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE termui::termui)
```

Then you can access all the interfaces by:
```c++
#include <termui/termui.hpp>
```

For examples of how to use the library, see ```test.cpp```.

## Data Management

Interfaces in this library are not responsible for owning and managing program data, and this extends to elements and structs. Instead, all library objects should store references to external program data. This seperation prevents data duplication, unintended side effects and mainly wasteful copy operations. Library objects can obviously own data when it is intrinsic to the object's state, this is mostly integers that represent dimensions or colors.

## Interfaces

Interfaces are the core of the library and designed to be plug and play wrappers around existing data. Interfaces are defined in ```include/interfaces.hpp``` and source is stored in ```src/interfaces```.

The following interfaces are implemented:

- BinaryMenu: yes/no dialog box
- Editor: simple text editor
- InfoBox: info popup
- InfoPage: scrollable text
- InputBox: input popup
- InputPage: multiple scrollable input fields
- Menu: one shot selection menu
- FancyMenu: looks nicer
- MultiMenu: multi shot selection menu
- Spreadsheet: like excel but terrible + no functions

## Elements

Interfaces often use elements, reusable UI components. Elements are defined in ```include/elements.hpp``` and source is stored in ```src/elements/```.

The following elements are available:

- Box
- Button
- DashedArea
- Input
- List
- FancyList (looks nicer ig)
- SelectList (multi shot selection)
- ProgressBar
- Table
- Text
