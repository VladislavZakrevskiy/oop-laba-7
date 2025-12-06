cmake_minimum_required(VERSION 3.16)
project(AsyncDungeonEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -pthread")

add_library(async_dungeon_lib
    src/NPC.cpp
    src/Orc.cpp
    src/Knight.cpp
    src/Bear.cpp
    src/NPCFactory.cpp
    src/BattleSystem.cpp
    src/MovementSystem.cpp
    src/Observer.cpp
    src/MapRenderer.cpp
    src/AsyncGame.cpp
)

target_include_directories(async_dungeon_lib PUBLIC include)

add_executable(async_dungeon_demo
    src/main.cpp
)

add_executable(renderer_demo
    src/renderer_demo.cpp
)

target_link_libraries(async_dungeon_demo async_dungeon_lib pthread)
target_link_libraries(renderer_demo async_dungeon_lib pthread)

find_package(GTest QUIET)

if(GTest_FOUND)
    enable_testing()
    
    add_executable(async_dungeon_tests
        tests/test_async_dungeon.cpp
    )
    
    target_link_libraries(async_dungeon_tests 
        async_dungeon_lib 
        GTest::gtest 
        GTest::gtest_main
        pthread
    )
    
    include(GoogleTest)
    gtest_discover_tests(async_dungeon_tests)
    
    message(STATUS "Google Test найден. Тесты будут собраны.")
else()
    message(WARNING "Google Test не найден. Тесты не будут собраны.")
    message(STATUS "Для установки Google Test выполните:")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install libgtest-dev")
    message(STATUS "  macOS: brew install googletest")
endif()

if(TARGET async_dungeon_tests)
    add_custom_target(run_tests
        COMMAND async_dungeon_tests
        DEPENDS async_dungeon_tests
        COMMENT "Запуск тестов"
    )
endif()
