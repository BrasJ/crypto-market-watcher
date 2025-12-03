#include "ui/dashboard_renderer.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <thread>

namespace cmw {

namespace {

std::string volatility_label(double vol) {
    if (vol < 0.5) {
        return "LOW";
    }
    if (vol < 1.5) {
        return "MED";
    }
    return "HIGH";
}

}

DashboardRenderer::DashboardRenderer(const MarketDataEngine& engine, const AlertEngine& alerts)
    : engine_(engine), alerts_(alerts) {}

void DashboardRenderer::start() {
    running_ = true;
    worker_ = std::thread([this]{ run(); });
}

void DashboardRenderer::stop() {
    running_ = false;
    if (worker_.joinable()) {
        worker_.join();
    }
}

void DashboardRenderer::run() {
    using namespace std::chrono_literals;

    while (running_) {
        auto snapshot = engine_.snapshot_all();
        auto recent_alerts = alerts_.get_recent_alerts();

        std::cout << "\033[2J\033[H";  // clear screen

        std::cout << "SYMBOL   "
                  << "PRICE     "
                  << "1M%    "
                  << "5M%    "
                  << "MA_S    "
                  << "MA_L    "
                  << "VOL_1M\n";
        std::cout << "----------------------------------------------------------\n";
        
        std::cout << std::fixed << std::setprecision(2);

        for (const auto& [symbol, metrics] : snapshot) {
            std::cout << std::left << std::setw(8) << symbol << " "
                      << std::setw(8) << metrics.last_price << " "
                      << std::setw(6) << metrics.pct_change_1m << " "
                      << std::setw(6) << metrics.pct_change_5m << " "
                      << std::setw(7) << metrics.ma_short << " "
                      << std::setw(7) << metrics.ma_long << " "
                      << volatility_label(metrics.volatility_1m)
                      << "\n";
        }

        std::cout << "\nRecent alerts: " << recent_alerts.size() << "\n";
        for (const auto& alert : recent_alerts) {
            std::cout << " - [" << alert.rule_id << "] "
                      << alert.symbol << ": "
                      << alert.message << "\n";
        }

        std::this_thread::sleep_for(500ms);
    }
}

} // namespace cmw
