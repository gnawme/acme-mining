/// \file   MineLogger.h
/// \brief  Simple logging singleton
#pragma once
#include "AcmeMinerUtils.h"

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
        std::clog << msg << std::endl;

        std::lock_guard<std::mutex> lockGuard(_mutex);
        _logfile << msg << std::endl;
    }

    ///
    MineLogger(const MineLogger&) = delete;
    MineLogger& operator=(const MineLogger&) = delete;

private:
    ///
    MineLogger() {
        auto dateStamp = createISODateStamp();
        std::string acmeLog(dateStamp + "_AcmeMinerSim.log");
        _logfile.open(acmeLog, std::ios::app);
    };

    ///
    ~MineLogger() {
        if (_logfile.is_open()) {
            _logfile.close();
        }
    }

    std::mutex _mutex;
    std::ofstream _logfile;
};
}  // namespace acme