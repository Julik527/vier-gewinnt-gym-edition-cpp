# JK AXIS | GOLD MASTER
# Projekt: Vier Gewinnt | Gym Edition
# Datei:   CMakeLists.txt
# Aufgabe: Baut das C++-Spiel und führt die automatischen Tests aus.
# ------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
project(VierGewinntGymEdition LANGUAGES CXX)

# Wir nutzen C++17, damit der Code mit den geforderten Sprachfunktionen laeuft.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Die kleine Fall-Animation braucht die Thread-Bibliothek.
find_package(Threads REQUIRED)

# Board ist eine eigene Bibliothek. So koennen Spiel und Tests sie gemeinsam nutzen.
add_library(gym_board STATIC src/Board.cpp)
target_include_directories(gym_board PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(gym_board PUBLIC Threads::Threads)

# Hier entsteht das eigentliche Konsolenspiel.
add_executable(vier-gewinnt src/main.cpp src/Game.cpp)
target_link_libraries(vier-gewinnt PRIVATE gym_board)

# Dieses zweite Programm testet nur die Regeln des Spielfelds.
add_executable(board_tests tests/board_tests.cpp)
target_link_libraries(board_tests PRIVATE gym_board)

foreach(target gym_board vier-gewinnt board_tests)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endforeach()

# Mit CTest lassen sich alle eingetragenen Tests zusammen starten.
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
