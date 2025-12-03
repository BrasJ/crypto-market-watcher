add_executable(test_core
    test_market_data.cpp
    test_alert.cpp
    test_queue.cpp
)

target_link_libraries(test_core
    PRIVATE
        core
        util
        Catch2::Catch2WithMain
)

add_test(NAME test_core COMMAND test_core)
