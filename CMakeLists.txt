cmake_minimum_required(VERSION 3.28.0)
project(termui LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE TERMUI_SOURCES "src/*.cpp")
file(GLOB TERMUI_INCLUDE "include/*.hpp")

# message("termui sources: ${TERMUI_SOURCES}")
# message("termui includes: ${TERMUI_INCLUDE}")

add_library(termui ${TERMUI_SOURCES})
target_include_directories(termui PRIVATE include)
target_compile_options(termui PRIVATE -Wall -Wextra -Wuninitialized -Wno-sign-compare)

set_target_properties(termui
    PROPERTIES
    CMAKE_CXX_STANDARD 20
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

include(GNUInstallDirs)

target_include_directories(termui
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS termui
    EXPORT termuiTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${TERMUI_INCLUDE} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/termui)

install(EXPORT termuiTargets
    FILE termuiTargets.cmake
    NAMESPACE termui::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/termui
)

include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/termuiConfig.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/termui
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/termuiConfig.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/termui
)

add_executable(test_program test.cpp)
target_link_libraries(test_program PRIVATE termui)
target_compile_options(test_program PRIVATE -Wall -Wextra -Wuninitialized)
