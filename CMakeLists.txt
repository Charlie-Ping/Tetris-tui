cmake_minimum_required (VERSION 3.2)

project (Internship_20231614134)

include_directories(include)
file(GLOB SOURCES "src/*.c")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -pie")

set(EXTERNAL_DIR "${CMAKE_SOURCE_DIR}/external")
set(WIN_BIN "${CMAKE_SOURCE_DIR}/build/win/bin")
set(WIN_LIB "${CMAKE_SOURCE_DIR}/build/win/lib")
set(LINUX_LIB "${CMAKE_SOURCE_DIR}/build/linux/lib")
set(LINUX_BIN "${CMAKE_SOURCE_DIR}/build/linux/bin")

file(COPY "${EXTERNAL_DIR}/PDCurses/curses.h" DESTINATION "${CMAKE_SOURCE_DIR}/include")

if (WIN32)
# Windows
    # 读取 Makefile
    set(PDCURSES_MAKEFILE_PATH "${EXTERNAL_DIR}/PDCurses/wincon/Makefile")
    
    file(READ ${PDCURSES_MAKEFILE_PATH} MAKEFILE_CONTENTS)

    # 替换编译器设置
    string(REPLACE "CC		= gcc" "CC		= x86_64-w64-mingw32-gcc" MAKEFILE_CONTENTS "${MAKEFILE_CONTENTS}")
    string(REPLACE "LINK		= gcc" "LINK		= x86_64-w64-mingw32-gcc" MAKEFILE_CONTENTS "${MAKEFILE_CONTENTS}")
    string(REPLACE "AR		= ar" "AR		= x86_64-w64-mingw32-ar" MAKEFILE_CONTENTS "${MAKEFILE_CONTENTS}")
    string(REPLACE "STRIP		= strip" "STRIP		= x86_64-w64-mingw32-strip" MAKEFILE_CONTENTS "${MAKEFILE_CONTENTS}")
    string(REPLACE "WINDRES		= windres" "WINDRES		= x86_64-w64-mingw32-windres" MAKEFILE_CONTENTS "${MAKEFILE_CONTENTS}")
    file(WRITE ${PDCURSES_MAKEFILE_PATH} "${MAKEFILE_CONTENTS}")

    # 编译pdcurses
    add_custom_target(
        PDCurses_wincon ALL
        COMMAND make -C ${EXTERNAL_DIR}/PDCurses/wincon TOOLCHAIN_PREFIX=${TOOLCHAIN_PREFIX} DLL=Y WIDE=Y UTF8=Y
        WORKING_DIRECTORY ${EXTERNAL_DIR}/PDCurses/wincon
    )

    add_custom_command(TARGET PDCurses_wincon POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/PDCurses/wincon/pdcurses.dll ${WIN_LIB}/pdcurses.dll
        COMMAND ${CMAKE_COMMAND} -E copy ${EXTERNAL_DIR}/PDCurses/wincon/pdcurses.a ${WIN_LIB}/pdcurses.a
    )

    add_library(pdcurses_win STATIC IMPORTED)
    set_target_properties(pdcurses_win PROPERTIES IMPORTED_IMPLIB "{WIN_LIB}/pdcurses.a" IMPORTED_LOCATION "${WIN_LIB}/pdcurses.dll")

    # 添加链接库
    add_executable(Tetris-win-x64 ${SOURCES})
    # 确保 Tetris-win-x64 在 pdcurses.dll 完成构建之后才开始构建。
    add_dependencies(Tetris-win-x64 PDCurses_wincon)
    set_target_properties(Tetris-win-x64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${WIN_BIN}")
    target_link_libraries(Tetris-win-x64 PRIVATE pdcurses_win)
else()
# Linux
    include(ExternalProject)

    # 编译pdcurses for x11
    ExternalProject_Add(
        PDCurses_x11
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/PDCurses/x11
        CONFIGURE_COMMAND ${CMAKE_SOURCE_DIR}/external/PDCurses/x11/configure --enable-force-utf8
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        INSTALL_COMMAND ""
    )

    # 创建动态链接库
    ExternalProject_Get_Property(PDCurses_x11 SOURCE_DIR BINARY_DIR)
    add_custom_command(
        TARGET PDCurses_x11 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libXCurses.a ${LINUX_LIB}/libXCurses.a
        COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libXCurses.so ${LINUX_LIB}/libXCurses.so
    )

    add_library(pdcurses_x11 SHARED IMPORTED)
    set_target_properties(pdcurses_x11 PROPERTIES IMPORTED_LOCATION "${LINUX_LIB}/libXCurses.so")

    #添加链接库
    add_executable(Tetris-linux-x64 ${SOURCES})
    set_target_properties(Tetris-linux-x64 PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${LINUX_BIN}")
    add_dependencies(Tetris-linux-x64 PDCurses_x11)
    target_link_libraries(Tetris-linux-x64 PRIVATE pdcurses_x11)
endif()

