cmake_minimum_required(VERSION 3.15)

# parent already defines the termui target.
# this file builds every example under examples/

message(STATUS "Configuring examples...")

# get all subdirectories inside examples/
file(GLOB EXAMPLE_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)

foreach(example_dir ${EXAMPLE_DIRS})
    set(example_path "${CMAKE_CURRENT_SOURCE_DIR}/${example_dir}")
    if(IS_DIRECTORY "${example_path}")
        file(GLOB EXAMPLE_SOURCES "${example_path}/*.cpp") # collect all .cpp files in the example folder (usually just 1)

        if(EXAMPLE_SOURCES)
            add_executable(${example_dir} ${EXAMPLE_SOURCES}) # create an executable target named after the folder
            target_link_libraries(${example_dir} PRIVATE termui)
            message(STATUS "Added example target: ${example_dir}")
        endif()

    endif()
endforeach()
