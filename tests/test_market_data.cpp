#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "core/market_data_engine.hpp"
#include "core/types.hpp"

#include <chrono>

TEST_CASE("MarketDataEngine computes basic metrics", "[market_data]") {
    cmw::MarketDataEngine engine;

    auto now = std::chrono::system_clock::now();

    cmw::MarketEvent ev1{"BTCUSDT", 100.0, now};
    cmw::MarketEvent ev2{"BTCUSDT", 110.0, now + std::chrono::seconds(1)};
    cmw::MarketEvent ev3{"BTCUSDT", 120.0, now + std::chrono::seconds(2)};

    engine.update(ev1);
    engine.update(ev2);
    engine.update(ev3);

    auto snapshot = engine.snapshot_all();
    REQUIRE(snapshot.size() == 1);

    const auto& [symbol, metrics] = snapshot[0];

    REQUIRE(symbol == "BTCUSDT");
    REQUIRE(metrics.last_price == Catch::Approx(120.0));

    // With only 3 ale and large window capacities,
    // both short and long MAs are just the average of the three prices.
    const double expected_avg = (100.0 + 110.0 + 120.0) / 3.0;
    REQUIRE(metrics.ma_short == Catch::Approx(expected_avg));
    REQUIRE(metrics.ma_long == Catch::Approx(expected_avg));

    // Percent change: (120 - 100) / 100 * 100 = 20%
    REQUIRE(metrics.pct_change_1m == Catch::Approx(20.0));
    REQUIRE(metrics.pct_change_5m == Catch::Approx(20.00));

    // Volatility should be nonnegative
    REQUIRE(metrics.volatility_1m >= 0.0);
}
