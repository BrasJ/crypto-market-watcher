#include "ui/dashboard_renderer.hpp"
#include <chrono>
#include <iostream>
#include <thread>

namespace cmw {

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

        std::cout << "\033[2J\033[H";
        std::cout << "SYMBOL        PRICE\n";
        std::cout << "---------------------\n";
        for (const auto& [symbol, metrics] : snapshot) {
            std::cout << symbol << "    " << metrics.last_price << "\n";
        }
        std::cout << "\nRecent alerts: " << recent_alerts.size() << "\n";
        std::this_thread::sleep_for(500ms);
    }
}

} // namespace cmw
