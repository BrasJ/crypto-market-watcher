#include "core/alert_engine.hpp"

#include <chrono>
#include <cmath>

namespace cmw {

// ---- PriceThresholdRule ----

PriceThresholdRule::PriceThresholdRule(std::string symbol,
                                       double threshold,
                                       ThresholdDirection direction)
    : symbol_(std::move(symbol)),
      threshold_(threshold),
      direction_(direction) {}

bool PriceThresholdRule::evaluate(const SymbolMetrics& metrics, Alert& out_alert) {
    const double price = metrics.last_price;

    bool triggered = false;
    if (direction_ == ThresholdDirection::AtOrAbove) {
        triggered = price >= threshold_;
    } else {
        triggered = price <= threshold_;
    }

    if (!triggered) {
        return false;
    }

    out_alert.rule_id = id();
    out_alert.message = description();
    out_alert.timestamp = std::chrono::system_clock::now();
    return true;
}

std::string PriceThresholdRule::description() const {
    std::string dir = (direction_ == ThresholdDirection::AtOrAbove)
                          ? ">= "
                          : "<= ";
    return "Price " + dir + std::to_string(threshold_);
}

// ---- PercentChangeRule ----

PercentChangeRule::PercentChangeRule(std::string symbol,
                                     double min_change_1m,
                                     bool use_absolute)
    : symbol_(std::move(symbol)),
      min_change_1m_(min_change_1m),
      use_absolute_(use_absolute) {}

bool PercentChangeRule::evaluate(const SymbolMetrics& metrics, Alert& out_alert) {
    double change = metrics.pct_change_1m;
    if (use_absolute_) {
        change = std::fabs(change);
    }

    if (change < min_change_1m_) {
        return false;
    }

    out_alert.rule_id = id();
    out_alert.message = description();
    out_alert.timestamp = std::chrono::system_clock::now();
    return true;
}

std::string PercentChangeRule::description() const {
    return "1m % change >= " + std::to_string(min_change_1m_);
}

// ---- AlertEngine ----

void AlertEngine::add_rule(std::unique_ptr<AlertRule> rule) {
    rules_.push_back(std::move(rule));
}

void AlertEngine::evaluate(const std::string& symbol, const SymbolMetrics& metrics) {
    for (auto& rule : rules_) {
        Alert alert;
        if (rule->evaluate(metrics, alert)) {
            alert.symbol = symbol;
            std::lock_guard lock(alerts_mutex_);
            recent_alerts_.push_back(alert);
            if (recent_alerts_.size() > 100) {
                recent_alerts_.erase(recent_alerts_.begin());
            }
        }
    }
}

std::vector<Alert> AlertEngine::get_recent_alerts() const {
    std::lock_guard lock(alerts_mutex_);
    return recent_alerts_;
}

} // namespace cmw