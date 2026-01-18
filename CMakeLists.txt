cmake_minimum_required(VERSION 3.28)

project(mctetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Curses REQUIRED)

add_executable(mctetris
    src/main.cpp
    src/model/board.cpp
    src/model/game_model.cpp
    src/model/tetromino.cpp
)

target_include_directories(mctetris PRIVATE ${CURSES_INCLUDE_DIR})
target_link_libraries(mctetris PRIVATE ${CURSES_LIBRARIES})

find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE MCTETRIS_FORMAT_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.cc
        ${CMAKE_SOURCE_DIR}/src/*.cxx
    )
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${MCTETRIS_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-format on C++ sources"
    )
else()
    message(STATUS "clang-format not found; 'format' target disabled.")
endif()
