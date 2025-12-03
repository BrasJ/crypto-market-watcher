#include "core/alert_engine.hpp"

namespace cmw {

bool DummyAlertRule::evaluate(const SymbolMetrics& metrics, Alert& out_alert) {
    (void)metrics;
    (void)out_alert;
    return false;
}

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
