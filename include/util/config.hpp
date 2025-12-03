#pragma once
#include <string>
#include <vector>

namespace cmw {

struct AppConfig {
    std::string websocket_url;
    std::vector<std::string> symbols;
};

AppConfig load_default_config();

} // namespace cmw
