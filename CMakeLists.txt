cmake_minimum_required(VERSION 3.24)

project(KeyboardShortcuts VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add your executable
add_executable(Keyboard
    main.cpp
    keyboard.cpp
    shortcuts.cpp
    shortcuts.h
    keyboard.h
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBEVDEV REQUIRED libevdev)
target_include_directories(Keyboard PRIVATE ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(Keyboard PRIVATE ${LIBEVDEV_LIBRARIES})

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Keyboard PRIVATE -Wall -Wextra -Wpedantic)
endif()
