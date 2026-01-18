cmake_minimum_required(VERSION 3.28)

project(mctetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Curses REQUIRED)

add_executable(mctetris
    src/main.cpp
)

target_include_directories(mctetris PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(mctetris PRIVATE ${CURSES_LIBRARIES})
