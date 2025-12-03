#pragma once
#include "core/alert_engine.hpp"
#include "core/market_data_engine.hpp"
#include <atomic>
#include <thread>

namespace cmw {

class DashboardRenderer {
public:
    DashboardRenderer(const MarketDataEngine& engine, const AlertEngine& alerts);
    void start();
    void stop();

private:
    void run();
    const MarketDataEngine& engine_;
    const AlertEngine& alerts_;
    std::atomic<bool> running_{false};
    std::thread worker_;
};

} // namespace cmw
