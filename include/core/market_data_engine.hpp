#pragma once
#include "core/types.hpp"
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace cmw {

class MarketDataEngine {
public:
    void update(const MarketEvent& ev);
    std::vector<std::pair<std::string, SymbolMetrics>> snapshot_all() const;

private:
    struct SymbolState {
        SymbolMetrics metrics;
        void update(const MarketEvent& ev) {
            metrics.last_price = ev.price;
        }
    };

    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, SymbolState> symbols_;
};

} // namespace cmw
