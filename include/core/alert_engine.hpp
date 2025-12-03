#pragma once
#include "core/types.hpp"
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace cmw {

class AlertRule {
public:
    virtual ~AlertRule() = default;
    virtual bool evaluate(const SymbolMetrics& metrics, Alert& out_alert) = 0;
    virtual std::string id() const = 0;
};

class DummyAlertRule : public AlertRule {
public:
    bool evaluate(const SymbolMetrics& metrics, Alert& out_alert) override;
    std::string id() const override { return "dummy"; }
};

class AlertEngine {
public:
    void add_rule(std::unique_ptr<AlertRule> rule);
    void evaluate(const std::string& symbol, const SymbolMetrics& metrics);
    std::vector<Alert> get_recent_alerts() const;

private:
    std::vector<std::unique_ptr<AlertRule>> rules_;
    mutable std::mutex alerts_mutex_;
    std::vector<Alert> recent_alerts_;
};

} // namespace cmw
