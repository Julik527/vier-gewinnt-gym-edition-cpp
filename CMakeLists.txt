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
add_executable(vier-gewinnt src/main.cpp src/Game.cpp src/ConsoleUI.cpp)
target_link_libraries(vier-gewinnt PRIVATE gym_board)

# Dieses zweite Programm testet nur die Regeln des Spielfelds.
add_executable(board_tests tests/board_tests.cpp)
target_link_libraries(board_tests PRIVATE gym_board)

# Das zweite Testprogramm kontrolliert die Textoberflaeche.
add_executable(console_ui_tests tests/console_ui_tests.cpp src/ConsoleUI.cpp)
target_link_libraries(console_ui_tests PRIVATE gym_board)

# GUI-unabhaengiger Spielzustand: kann auch ohne grafisches Display getestet werden.
add_library(gym_match STATIC src/MatchSession.cpp)
target_link_libraries(gym_match PUBLIC gym_board)
target_include_directories(gym_match PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
add_executable(match_session_tests tests/match_session_tests.cpp)
target_link_libraries(match_session_tests PRIVATE gym_match)

# Die GUI ist optional, damit das vorhandene Konsolenspiel ohne SFML baubar bleibt.
option(GYM_BUILD_GUI "Build the standalone SFML 3.1 graphical Gym Edition" OFF)
if(GYM_BUILD_GUI)
    if(CMAKE_VERSION VERSION_LESS 3.28)
        message(FATAL_ERROR "The SFML 3.1 GUI build requires CMake 3.28 or later.")
    endif()
    find_package(SFML 3.1 COMPONENTS Graphics QUIET)
    if(NOT SFML_FOUND)
        include(FetchContent)
        set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
        set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
        set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
        FetchContent_Declare(SFML
            GIT_REPOSITORY https://github.com/SFML/SFML.git
            GIT_TAG 3.1.0
            GIT_SHALLOW ON
            EXCLUDE_FROM_ALL
            SYSTEM)
        FetchContent_MakeAvailable(SFML)
    endif()
    add_executable(vier-gewinnt-gui src/gui/Main.cpp)
    target_link_libraries(vier-gewinnt-gui PRIVATE gym_match SFML::Graphics)
    if(WIN32)
        # A Windows console remains useful for early errors and diagnostics.
        set_target_properties(vier-gewinnt-gui PROPERTIES WIN32_EXECUTABLE FALSE)
    endif()
endif()

foreach(target gym_board vier-gewinnt board_tests console_ui_tests gym_match match_session_tests)
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
    add_test(NAME match_session_unit_tests COMMAND match_session_tests)
    if(GYM_BUILD_GUI)
        add_test(NAME gui_core_smoke COMMAND vier-gewinnt-gui --self-test)
    endif()
    add_test(NAME console_ui_unit_tests COMMAND console_ui_tests)
    find_package(Python3 COMPONENTS Interpreter)
    if(Python3_Interpreter_FOUND)
        add_test(
            NAME game_integration_tests
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/integration_smoke.py" "$<TARGET_FILE:vier-gewinnt>"
        )
        set_tests_properties(game_integration_tests PROPERTIES TIMEOUT 60)
        # Verhindert kaputte F5-Konfigurationen nach neuen Quelldateien.
        add_test(
            NAME vscode_config_tests
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/vscode_config_test.py" "${CMAKE_CURRENT_SOURCE_DIR}"
        )
    else()
        message(WARNING "Python3 missing: interactive integration test not registered")
    endif()
endif()
