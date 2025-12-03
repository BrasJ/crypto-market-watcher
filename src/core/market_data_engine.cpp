#include "core/market_data_engine.hpp"

#include <cmath>
#include <mutex>

namespace cmw {
// ---- SymbolState implementation ----

MarketDataEngine::SymbolState::SymbolState()
    : short_window_(SHORT_CAPACITY),
      long_window_(LONG_CAPACITY) {}

void MarketDataEngine::SymbolState::update(const MarketEvent& ev) {
    metrics.last_price = ev.price;

    Sample sample{ev.price, ev.timestamp};
    short_window_.push_back(sample);
    long_window_.push_back(sample);

    compute_metrics();
}

void MarketDataEngine::SymbolState::compute_metrics() {
    metrics.ma_short = compute_average(short_window_);
    metrics.ma_long = compute_average(long_window_);

    metrics.pct_change_1m = compute_pct_change(short_window_);
    metrics.pct_change_5m = compute_pct_change(long_window_);
    
    metrics.volatility_1m = compute_volatility(short_window_);
}

double MarketDataEngine::SymbolState::compute_average(const RingBuffer<Sample>& window) {
    if (window.size() == 0) {
        return 0.0;
    }

    double sum = 0.0;
    for (std::size_t i = 0; i < window.size(); i++) {
        sum += window[i].price;
    }
    return sum / static_cast<double>(window.size());
}

double MarketDataEngine::SymbolState::compute_pct_change(const RingBuffer<Sample>& window) {
    if (window.size() < 2) {
        return 0.0;
    }


    const double first = window[0].price;
    const double last = window[window.size() - 1].price;
    if (first == 0.0) {
        return 0.0;
    }

    return (last - first) / first * 100.0;
}

double MarketDataEngine::SymbolState::compute_volatility(const RingBuffer<Sample>& window) {
    if (window.size() < 2) {
        return 0.0;
    }

    const double mean = compute_average(window);

    double sum_sq = 0.0;
    for (std::size_t i = 0; i < window.size(); i++) {
        const double diff  = window[i].price - mean;
        sum_sq += diff * diff;
    }

    const double variance = sum_sq / static_cast<double>(window.size() - 1);
    return std::sqrt(variance);
}

// ---- MarketDataEngine imlementation ----

void MarketDataEngine::update(const MarketEvent& ev) {
    std::unique_lock<std::shared_mutex> lock(mutex_);
    auto& state = symbols_[ev.symbol];
    state.update(ev);
}

std::vector<std::pair<std::string, SymbolMetrics>> MarketDataEngine::snapshot_all() const {
    std::shared_lock lock(mutex_);
    std::vector<std::pair<std::string, SymbolMetrics>> result;
    result.reserve(symbols_.size());
    for (const auto& [symbol, state] : symbols_) {
        result.emplace_back(symbol, state.metrics);
    }
    return result;
}

SymbolMetrics MarketDataEngine::get_metrics(const std::string& symbol) const {
    std::shared_lock<std::shared_mutex> lock(mutex_);
    auto it = symbols_.find(symbol);
    if (it == symbols_.end()) {
        return SymbolMetrics{};
    }
    return it->second.metrics;
}

} // namespace cmw
