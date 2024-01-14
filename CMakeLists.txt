cmake_minimum_required (VERSION 3.2)

project (Internship_20231614134)

include(ExternalProject)

file(GLOB SOURCES "src/*.c")
set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")
set(WIN_BIN "${CMAKE_SOURCE_DIR}/build/win/bin")
set(LINUX_BIN "${CMAKE_SOURCE_DIR}/build/linux/bin")


add_subdirectory("${EXTERNAL_DIR}/notcurses")

if (WIN32)
    add_executable(Tetris-win-x64 ${SOURCES})
    set_target_properties(Tetris-win-x64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${WIN_BIN}")
    target_link_libraries(Tetris-win-x64 PRIVATE notcurses)
else()
    add_executable(Tetris-linux-x64 ${SOURCES})
    set_target_properties(Tetris-linux-x64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${LINUX_BIN}")
    target_link_libraries(Tetris-linux-x64 PRIVATE notcurses)
endif()