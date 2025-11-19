include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        master 
)
FetchContent_MakeAvailable(pybind11)

file(GLOB SOURCES "*.cpp")

add_library(termui_python MODULE ${SOURCES})

target_link_libraries(termui_python
    PRIVATE
        pybind11::module
        termui      
)

pybind11_extension(termui_python)

set_target_properties(termui_python
    PROPERTIES
        OUTPUT_NAME "termui"
)
