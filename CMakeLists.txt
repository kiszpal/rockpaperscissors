cmake_minimum_required(VERSION 3.10)

project(RockPaperScissors C)

set(CMAKE_C_STANDARD 99 CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package(CUNIT)

add_library(RPSEngine SHARED engine/engine.h engine/engine.c engine/ai.h engine/ai.c)

add_executable(RockPaperScissors src/main.c engine/ANSI_colors.h)
add_executable(TestA src/utest.c src/utest_engine.h src/utest_engine.c)


target_link_libraries(RockPaperScissors RPSEngine ${CURSES_LIBRARIES})
target_link_libraries(TestA RPSEngine /usr/lib/libcunit.so)


target_include_directories(RockPaperScissors PUBLIC "${PROJECT_SOURCE_DIR}/engine")
target_include_directories(TestA PUBLIC "${PROJECT_SOURCE_DIR}/engine" ${CUNIT_INCLUDE_DIRS})

