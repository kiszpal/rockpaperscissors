cmake_minimum_required(VERSION 3.10)
project(RockPaperScissors C)

set(CMAKE_C_STANDARD 99)

add_library(RPSEngine SHARED engine/engine.h engine/engine.c)

add_executable(RockPaperScissors src/main.c)

target_link_libraries(RockPaperScissors RPSEngine)

target_include_directories(RockPaperScissors PUBLIC
        "${PROJECT_SOURCE_DIR}/engine")