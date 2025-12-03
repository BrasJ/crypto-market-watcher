#pragma once
#include <iostream>
#include <string>

namespace cmw {

inline void log_info(const std::string& msg) {
    std::cerr << "[INFO] " << msg << '\n';
}

inline void log_error(const std::string& msg) {
    std::cerr << "[ERROR] " << msg << '\n';
}

} // namespace cmw
