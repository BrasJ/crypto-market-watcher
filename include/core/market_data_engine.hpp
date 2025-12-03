#pragma once

#include "core/ring_buffer.hpp"
#include "core/types.hpp"

#include <chrono>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace cmw {

class MarketDataEngine {
public:
    void update(const MarketEvent& ev);
    std::vector<std::pair<std::string, SymbolMetrics>> snapshot_all() const;
    SymbolMetrics get_metrics(const std::string& symbol) const;

private:
    struct Sample {
        double price {0.0};
        std::chrono::system_clock::time_point ts{};
    };
    struct SymbolState {
        SymbolState();

        void update(const MarketEvent& ev);
        
        SymbolMetrics metrics;

    private:
        static constexpr std::size_t SHORT_CAPACITY = 60; // about 1m worth
        static constexpr std::size_t LONG_CAPACITY = 300; // about 5m worth

        RingBuffer<Sample> short_window_;
        RingBuffer<Sample> long_window_;

        void compute_metrics();
        static double compute_average(const RingBuffer<Sample>& window);
        static double compute_pct_change(const RingBuffer<Sample>& window);
        static double compute_volatility(const RingBuffer<Sample>& window);
    };

    mutable std::shared_mutex mutex_;
    std::unordered_map<std::string, SymbolState> symbols_;
};

} // namespace cmw
