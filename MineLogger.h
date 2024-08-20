/// \file   MineLogger.h
/// \brief  Simple logging singleton
#pragma once

#include <fstream>
#include <iostream>
#include <mutex>
#include <string>

namespace acme {
///
class MineLogger {
public:
    ///
    static MineLogger& getInstance() {
        static MineLogger instance;
        return instance;
    }

    ///
    void logMessage(const std::string& msg) {
        if (!_logToFile) {
            std::clog << msg << std::endl;
        } else {
            std::lock_guard<std::mutex> lockGuard(_mutex);
            _logfile << msg << std::endl;
        }
    }

    ///
    MineLogger(const MineLogger&) = delete;
    MineLogger& operator=(const MineLogger&) = delete;

private:
    ///
    MineLogger(bool logToFile = false) {
        if (_logToFile) {
            const char ACME_LOG[]{"AcmeMinerSim.log"};
            _logfile.open(ACME_LOG, std::ios::app);
        }
    };

    ///
    ~MineLogger() {
        if (_logToFile) {
            if (_logfile.is_open()) {
                _logfile.close();
            }
        }
    }

    bool _logToFile{false};
    std::mutex _mutex;
    std::ofstream _logfile;
};
}  // namespace acme