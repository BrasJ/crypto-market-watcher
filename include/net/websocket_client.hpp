#pragma once
#include "core/event_queue.hpp"
#include "core/types.hpp"
#include <atomic>
#include <string>
#include <thread>
#include <vector>

namespace cmw {

class IWebSocketClient {
public:
    virtual ~IWebSocketClient() = default;
    virtual void connect(const std::string& uri) = 0;
    virtual void subscribe(const std::vector<std::string>& symbols) = 0;
    virtual void run() = 0;
    virtual void stop() = 0;
};

class DummyWebSocketClient : public IWebSocketClient {
public:
    explicit DummyWebSocketClient(ThreadSafeQueue<MarketEvent>& queue);
    void connect(const std::string& uri) override;
    void subscribe(const std::vector<std::string>& symbols) override;
    void run() override;
    void stop() override;

private:
    ThreadSafeQueue<MarketEvent>& queue_;
    std::atomic<bool> running_{false};
    std::vector<std::string> symbols_;
};

} // namespace cmw
