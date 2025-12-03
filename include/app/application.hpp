#pragma once
#include "core/event_queue.hpp"
#include "core/market_data_engine.hpp"
#include "core/types.hpp"
#include "net/websocket_client.hpp"
#include "ui/dashboard_renderer.hpp"
#include "util/config.hpp"
#include <atomic>
#include <memory>
#include <thread>

namespace cmw {

class Application {
public:
    Application();
    int run();

private:
    void start_network_thread();
    void start_processing_thread();
    void stop_all();

    AppConfig config_;
    ThreadSafeQueue<MarketEvent> event_queue_;
    MarketDataEngine market_engine_;
    AlertEngine alert_engine_;
    DashboardRenderer dashboard_;
    std::unique_ptr<IWebSocketClient> websocket_client_;
    std::thread network_thread_;
    std::thread processing_thread_;
    std::atomic<bool> running_{false};
};

} // namespace cmw
