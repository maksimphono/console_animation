# install ncurses and ncurses-dev:
# apk add ncurses
# apk add ncurses-dev

cmake_minimum_required(VERSION 3.10)

# Project name
project(conanim)

set(CMAKE_CXX_STANDARD 20) # use C++20 features
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)
set(THREADS_PREFER_PTHREAD_FLAG ON)

#add_compile_options(
#    -Wall          # Enable all common warnings
#    -Wextra        # Enable extra warnings
#    -Wpedantic     # Ensure strict ISO C++ compliance
#    -Werror        # Treat ALL warnings as ERRORS (This stops the build)
#)

# For older systems (CentOS 8, Debian 11)
find_package(Threads REQUIRED)

# Find the ncurses library
find_package(Curses REQUIRED)

# Add the executable
add_executable(conanim src/storage.cpp src/env_arguments.cpp src/frames.cpp src/render.cpp src/main.cpp)

# Link the executable to the ncurses library

target_include_directories(conanim PRIVATE ${CURSES_INCLUDE_DIR})


# For older systems (CentOS 8, Debian 11)
# Will forecefully link stdc++fs library (for module <filesystem>)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.0)
        target_link_libraries(conanim PRIVATE stdc++fs)
    elseif(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 10.0)
        target_link_libraries(conanim PRIVATE stdc++fs)
    endif()
endif()

# Add include directories for ncurses (if find_package didn't handle it automatically)
target_link_libraries(conanim PRIVATE ${CURSES_LIBRARIES})

target_link_libraries(conanim PRIVATE Threads::Threads)

# Install on the system
install(TARGETS conanim DESTINATION bin)
