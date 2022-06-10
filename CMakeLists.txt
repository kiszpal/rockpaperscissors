cmake_minimum_required(VERSION 3.10)

project(RockPaperScissors C)

set(CMAKE_C_STANDARD 99 CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(RPSEngine SHARED engine/engine.h engine/engine.c)

if (UNIX)
    set(CMAKE_EXE_LINKER_FLAGS "-rdynamic -lncurses") #kipal
endif()

add_executable(RockPaperScissors src/main.c engine/ANSI_colors.h)

target_link_libraries(RockPaperScissors RPSEngine ${CURSES_LIBRARIES})

target_include_directories(RockPaperScissors PUBLIC
        "${PROJECT_SOURCE_DIR}/engine")