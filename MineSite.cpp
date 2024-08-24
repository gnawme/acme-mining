/// \file   MineSite.cpp
/// \brief  Represents an H3 mining site
#include "MineSite.h"

#include "MineTimer.h"

#include <fstream>

namespace acme {
bool MineSite::_initial = true;
///
/// \param name
MineSite::MineSite(const std::string& name)
    : _siteName(name)
    , _timer(new MineTimer(H3_MINING_MIN, H3_MINING_MAX))
    , _duration((*_timer)()) {}

/// Returns a random mining time for this visit
int MineSite::getMiningDuration() {
    _duration = (*_timer)();
    return _duration;
}

/// Returns the mine's name
std::string MineSite::getName() const {
    return _siteName;
}

/// Outputs MineSite statistics
/// \param timestamp
void MineSite::outputStatistics(const std::string& timestamp) {
    std::string mineSiteOutput(timestamp + "_MineSite" + ".csv");
    std::ofstream siteOutput(mineSiteOutput, std::ios::app);

    if (MineSite::_initial) {
        siteOutput << "Mine,Idle,Mining" << std::endl;
        MineSite::_initial = false;
    }

    siteOutput << getName() << "," << (_idleCount * TICK_DURATION) << ","
               << (_miningCount * TICK_DURATION) << std::endl;
}

/// Set if a MineTruck is mining
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