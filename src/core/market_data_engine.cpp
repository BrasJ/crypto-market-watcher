#include "core/market_data_engine.hpp"

namespace cmw {

void MarketDataEngine::update(const MarketEvent& ev) {
    std::unique_lock lock(mutex_);
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

} // namespace cmw
