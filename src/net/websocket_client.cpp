#include "net/websocket_client.hpp"
#include "util/logger.hpp"
#include <chrono>
#include <thread>

namespace cmw {

DummyWebSocketClient::DummyWebSocketClient(ThreadSafeQueue<MarketEvent>& queue)
    : queue_(queue) {}

void DummyWebSocketClient::connect(const std::string& uri) {
    log_info("DummyWebSocketClient connecting to " + uri);
}

void DummyWebSocketClient::subscribe(const std::vector<std::string>& symbols) {
    symbols_ = symbols;
    log_info("DummyWebSocketClient subscribed to " + std::to_string(symbols_.size()) + " symbols");
}

void DummyWebSocketClient::run() {
    running_ = true;
    using namespace std::chrono_literals;
    double price = 100.0;

    while (running_) {
        for (const auto& sym : symbols_) {
            MarketEvent ev;
            ev.symbol = sym;
            ev.price = price;
            ev.timestamp = std::chrono::system_clock::now();
            queue_.push(ev);
            price += 0.1;
        }
        std::this_thread::sleep_for(500ms);
    }
}

void DummyWebSocketClient::stop() {
    running_ = false;
}

} // namespace cmw
