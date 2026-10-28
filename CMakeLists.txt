cmake_minimum_required(VERSION 3.16)
project(VierGewinntGymEdition LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(gym_board STATIC src/Board.cpp)
target_include_directories(gym_board PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_executable(vier-gewinnt src/main.cpp src/Game.cpp)
target_link_libraries(vier-gewinnt PRIVATE gym_board)

add_executable(board_tests tests/board_tests.cpp)
target_link_libraries(board_tests PRIVATE gym_board)

foreach(target gym_board vier-gewinnt board_tests)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

include(CTest)
if(BUILD_TESTING)
    add_test(NAME board_unit_tests COMMAND board_tests)
    find_package(Python3 COMPONENTS Interpreter)
    if(Python3_Interpreter_FOUND)
        add_test(
            NAME game_integration_tests
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_smoke.py" "$<TARGET_FILE:vier-gewinnt>"
        )
        set_tests_properties(game_integration_tests PROPERTIES TIMEOUT 60)
    else()
        message(WARNING "Python3 missing: interactive integration test not registered")
    endif()
endif()
