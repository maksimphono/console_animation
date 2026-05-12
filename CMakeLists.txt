# TODO: install ncurses and ncurses-dev:
# apk add ncurses
# apk add ncurses-dev
# Docker run:
# docker run -itd --rm ams21/cmake:release

cmake_minimum_required(VERSION 3.10)

# Project name
project(conanim)

set(CMAKE_CXX_STANDARD 20) # use C++20 features
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

#add_compile_options(
#    -Wall          # Enable all common warnings
#    -Wextra        # Enable extra warnings
#    -Wpedantic     # Ensure strict ISO C++ compliance
#    -Werror        # Treat ALL warnings as ERRORS (This stops the build)
#)

find_package(fmt REQUIRED)

# Find the ncurses library
# find_package will search for ncurses and set variables like NCURSES_LIBRARIES and NCURSES_INCLUDE_DIR
find_package(Curses REQUIRED)

# Add the executable
# mytui is the name of your executable
# mytui.cpp is your source file
add_executable(conanim src/storage.cpp src/env_arguments.cpp src/frames.cpp src/render.cpp src/main.cpp)


# Link the executable to the ncurses library
# target_link_libraries links the 'mytui' executable with the found ncurses libraries

target_include_directories(conanim PRIVATE ${CURSES_INCLUDE_DIR})

# Add include directories for ncurses (if find_package didn't handle it automatically, though it usually does)
target_link_libraries(conanim PRIVATE ${CURSES_LIBRARIES})

target_link_libraries(conanim PRIVATE fmt::fmt)

# Install on the system
install(TARGETS conanim DESTINATION bin)
