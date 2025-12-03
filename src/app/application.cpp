#include "app/application.hpp"
#include "core/alert_engine.hpp"
#include "util/logger.hpp"
#include <chrono>
#include <thread>

namespace cmw {

Application::Application()
    : config_(load_default_config()),
      dashboard_(market_engine_, alert_engine_) {
    websocket_client_ = std::make_unique<DummyWebSocketClient>(event_queue_);
    alert_engine_.add_rule(std::make_unique<DummyAlertRule>());
}

int Application::run() {
    running_ = true;
    websocket_client_->connect(config_.websocket_url);
    websocket_client_->subscribe(config_.symbols);
    start_network_thread();
    start_processing_thread();
    dashboard_.start();
    std::this_thread::sleep_for(std::chrono::seconds(5));
    stop_all();
    return 0;
}

void Application::start_network_thread() {
    network_thread_ = std::thread([this] {
        websocket_client_->run();
    });
}

void Application::start_processing_thread() {
    processing_thread_ = std::thread([this] {
        while (running_) {
            auto ev = event_queue_.wait_and_pop();
            market_engine_.update(ev);
        }
    });
}

void Application::stop_all() {
    running_ = false;
    websocket_client_->stop();
    if (network_thread_.joinable()) network_thread_.join();
    if (processing_thread_.joinable()) processing_thread_.join();
    dashboard_.stop();
    log_info("Application stopped.");
}

} // namespace cmw
