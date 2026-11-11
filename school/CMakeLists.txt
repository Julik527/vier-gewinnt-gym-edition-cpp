cmake_minimum_required(VERSION 3.16)
project(VierGewinntSchulabgabe LANGUAGES CXX)

# Standalone C++17 CONSOLE submission. No SFML, OpenGL or GUI targets.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
add_library(gym_board STATIC src/Board.cpp)
target_include_directories(gym_board PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(gym_board PUBLIC Threads::Threads)

add_executable(vier-gewinnt src/main.cpp src/Game.cpp src/ConsoleUI.cpp)
target_link_libraries(vier-gewinnt PRIVATE gym_board)

include(CTest)
if(BUILD_TESTING)
    add_executable(board_tests tests/board_tests.cpp)
    target_link_libraries(board_tests PRIVATE gym_board)

    add_executable(board_property_tests tests/board_property_tests.cpp)
    target_link_libraries(board_property_tests PRIVATE gym_board)

    add_executable(console_ui_tests tests/console_ui_tests.cpp src/ConsoleUI.cpp)
    target_link_libraries(console_ui_tests PRIVATE gym_board)

    add_executable(score_file_tests tests/score_file_tests.cpp)
    target_include_directories(score_file_tests PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

    add_test(NAME board_unit_tests COMMAND board_tests)
    add_test(NAME board_property_tests COMMAND board_property_tests)
    add_test(NAME console_ui_tests COMMAND console_ui_tests)
    add_test(NAME score_file_tests COMMAND score_file_tests)

    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME console_integration_smoke
            COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_smoke.py"
                "$<TARGET_FILE:vier-gewinnt>")
        add_test(NAME console_integration_adversarial
            COMMAND "${Python3_EXECUTABLE}"
                "${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_adversarial.py"
                "$<TARGET_FILE:vier-gewinnt>")
        set_tests_properties(console_integration_smoke PROPERTIES TIMEOUT 90)
        set_tests_properties(console_integration_adversarial PROPERTIES TIMEOUT 120)
    else()
        message(WARNING "Python3 fehlt: Konsolen-Integrationstests nicht registriert")
    endif()
endif()

foreach(target IN ITEMS gym_board vier-gewinnt board_tests board_property_tests console_ui_tests score_file_tests)
    if(TARGET ${target})
        if(MSVC)
            target_compile_options(${target} PRIVATE /W4)
        else()
            target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        endif()
    endif()
endforeach()
