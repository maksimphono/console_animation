# install ncurses and ncurses-dev:
# apk add ncurses
# apk add ncurses-dev

cmake_minimum_required(VERSION 3.10)

# Project name
project(Console animation)

set(CMAKE_CXX_STANDARD 20) # use C++20 features
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

# Find the ncurses library
find_package(Curses REQUIRED)

add_executable(main src/storage.cpp src/env_arguments.cpp src/frames.cpp src/render.cpp src/main.cpp)

# Link the executable to the ncurses library
target_link_libraries(main PRIVATE ${NCURSES_LIBRARIES})