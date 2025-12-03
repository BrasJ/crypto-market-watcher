#pragma once
#include <chrono>
#include <string>

namespace cmw {

struct MarketEvent {
    std::string symbol;
    double price{};
    std::chrono::system_clock::time_point timestamp{};
};

struct SymbolMetrics {
    double last_price{0.0};
    double ma_short{0.0};
    double ma_long{0.0};
    double pct_change_1m{0.0};
    double pct_change_5m{0.0};
    double volatility_1m {0.0};
};

struct Alert {
    std::string symbol;
    std::string rule_id;
    std::string message;
    std::chrono::system_clock::time_point timestamp{};
};

} // namespace cmw
