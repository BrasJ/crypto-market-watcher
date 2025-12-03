cmake_minimum_required(VERSION 3.16)
project(crypto-market-watcher LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build unit tests" ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

include(FetchContent)

if(BUILD_TESTING)
    FetchContent_Declare(
        catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.6.0
    )
    FetchContent_MakeAvailable(catch2)
endif()

add_library(core
    src/core/market_data_engine.cpp
    src/core/alert_engine.cpp
    src/core/recorder.cpp
)

target_include_directories(core PUBLIC include)

add_library(net
    src/net/websocket_client.cpp
)
target_include_directories(net PUBLIC include)
target_link_libraries(net PRIVATE core)

add_library(ui
    src/ui/dashboard_renderer.cpp
)
target_include_directories(ui PUBLIC include)
target_link_libraries(ui PRIVATE core)

add_library(util
    src/util/config.cpp
    src/util/logger.cpp
)
target_include_directories(util PUBLIC include)

add_executable(crypto-market-watcher
    src/app/main.cpp
    src/app/application.cpp
)

target_link_libraries(crypto-market-watcher
    PRIVATE core net ui util
)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
