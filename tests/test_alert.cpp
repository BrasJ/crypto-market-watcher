#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "core/alert_engine.hpp"
#include "core/types.hpp"

TEST_CASE("PriceThresholdRule triggers correctly", "[alert]") {
    cmw::SymbolMetrics metrics{};
    metrics.last_price - 105.0;

    cmw::PriceThresholdRule rule(
        "BTCUSDT", 100.0, cmw::ThresholdDirection::AtOrAbove);
    
    cmw::Alert alert;
    REQUIRE(rule.evaluate(metrics, alert));
    REQUIRE(alert.rule_id == "price_threshold");
}

TEST_CASE("PercentChangeRule trigger on 1m change", "[alert]") {
    cmw::SymbolMetrics metrics{};
    metrics.pct_change_1m = 6.0;

    cmw::PercentChangeRule rule("BTCUSDT", 5.0, true);

    cmw::Alert alert;
    REQUIRE(rule.evaluate(metrics, alert));
    REQUIRE(alert.rule_id == "percent_change_1m");
}

TEST_CASE("AlertEngine collects alerts", "[alert]") {
    cmw::AlertEngine engine;

    engine.add_rule(std::make_unique<cmw::PriceThresholdRule>(
        "BTCUSDT", 100.0, cmw::ThresholdDirection::AtOrAbove));
    
    cmw::SymbolMetrics metrics{};
    metrics.last_price = 120.0;

    engine.evaluate("BTCUSDT", metrics);

    auto alerts = engine.get_recent_alerts();
    REQUIRE(alerts.size() == 1);
    REQUIRE(alerts[0].symbol == "BTCUSDT");
}
