/// \file   MineSite.cpp
/// \brief  Represents an H3 mining site
#include "MineSite.h"

#include "MineTimer.h"

#include <fstream>

namespace acme {
///
/// \param name
MineSite::MineSite(const char* name)
    : _siteName(name)
    , _timer(new MineTimer(H3_MINING_MIN, H3_MINING_MAX))
    , _duration((*_timer)()) {}

/// Returns a random mining time for this visit
int MineSite::getMiningDuration() {
    _duration = (*_timer)();
    return _duration;
}

/// Returns the mine's name
const char* MineSite::getName() const {
    return _siteName.c_str();
}

///
/// \param timestamp
void MineSite::outputStatistics(const std::string& timestamp) {
    std::string mineSiteOutput("MineSite_" + timestamp + ".csv");
    std::ofstream siteOutput(mineSiteOutput, std::ios::app);
    siteOutput << getName() << "," << _idleCount << "," << _miningCount << std::endl;
}

///
void MineSite::setMiningFlag(bool beingMined) {
    _beingMined = beingMined;
}

///
/// \param timestamp
void MineSite::update(const std::string& timestamp) {
    _timestamp = timestamp;
    if (_beingMined) {
        ++_miningCount;
    } else {
        ++_idleCount;
    }
};
}  // namespace acme