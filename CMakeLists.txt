# TODO: install ncurses and ncurses-dev:
# apk add ncurses
# apk add ncurses-dev
# Docker run:
# docker run -itd --rm ams21/cmake:release

cmake_minimum_required(VERSION 3.10)

# Project name
project(Main CXX)

set(CMAKE_CXX_STANDARD 20) # use C++20 features
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

# Find the ncurses library
# find_package will search for ncurses and set variables like NCURSES_LIBRARIES and NCURSES_INCLUDE_DIR
find_package(Curses REQUIRED)

# Add the executable
# mytui is the name of your executable
# mytui.cpp is your source file
add_executable(main src/main.cpp)

# Link the executable to the ncurses library
# target_link_libraries links the 'mytui' executable with the found ncurses libraries
target_link_libraries(main PRIVATE ${NCURSES_LIBRARIES})

# Add include directories for ncurses (if find_package didn't handle it automatically, though it usually does)
# target_include_directories(main PRIVATE ${NCURSES_INCLUDE_DIR})
