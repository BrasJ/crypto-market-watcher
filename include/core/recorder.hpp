#pragma once

#include <string>

namespace cmw {

// Placeholder Recorder interface.
class Recorder {
public:
    explicit Recorder(const std::string& path)
        : path_(path) {}

    // Start recording in a background thread
    void start() {}

    // Stop recording and flush any buffered data
    void stop() {}

private:
    std::string path_;
};

}