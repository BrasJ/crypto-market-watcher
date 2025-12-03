#include "util/config.hpp"

namespace cmw {

AppConfig load_default_config() {
    AppConfig cfg;
    cfg.websocket_url = "wss://example.com/dummy";
    cfg.symbols = {"BTCUSDT", "ETHUSDT"};
    return cfg;
}

} // namespace cmw
