add_executable(test_core
    test_market_data_engine.cpp
    test_alert_engine.cpp
    test_event_queue.cpp
)

target_link_libraries(test_core
    PRIVATE
        core
        util
        Catch2::Catch2WithMain
)

add_test(NAME test_core COMMAND test_core)
