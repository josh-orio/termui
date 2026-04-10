# TermUI

Terminal interfaces library for C++.

[![CI](https://github.com/josh-orio/termui/actions/workflows/build.yml/badge.svg)](https://github.com/josh-orio/termui/actions/workflows/build.yml)

## Documentation

Almost all of the [docs/](docs/) are written using Ollama in Copilot Chat. I proofread them, but inaccuracies may persist.

## Get Started

**Requirements:** C++20, CMake

**Build & run demos:** ```make test```


**Install system-wide:** ``` make install```

**Using in your project with CMake (preferred):**
```cmake
include(FetchContent)
FetchContent_Declare(
  termui
  GIT_REPOSITORY https://github.com/josh-orio/termui.git
  GIT_TAG main
)
FetchContent_MakeAvailable(termui)

target_link_libraries(your_target PRIVATE termui)
```

Alternatively, if you want to use this library in a Python project, see [josh-orio/pytermui/](github.com/josh-orio/pytermui).
