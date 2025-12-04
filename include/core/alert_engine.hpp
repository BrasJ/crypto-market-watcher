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
    virtual std::string description() const = 0;
};

enum class ThresholdDirection {
    AtOrAbove,
    AtOrBelow,
};

class PriceThresholdRule : public AlertRule {
public:
    PriceThresholdRule(std::string symbol,
                       double threshold,
                       ThresholdDirection direction);

    bool evaluate(const SymbolMetrics& metrics, Alert& out_alert) override;
    std::string id() const override { return "price_threshold"; }
    std::string description() const override;

private:
    std::string symbol_;
    double threshold_{0.0};
    ThresholdDirection direction_;
};

class PercentChangeRule : public AlertRule {
public:
    PercentChangeRule(std::string symbol,
                      double min_change_1m,
                      bool use_absolute = true);

    bool evaluate(const SymbolMetrics& metrics, Alert& out_alert) override;
    std::string id() const override { return "percent_change_1m"; }
    std::string description() const override;

private:
    std::string symbol_;
    double min_change_1m_{0.0};
    bool use_absolute_{true};
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