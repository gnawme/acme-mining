/// \file   MineOverlord.cpp
#include "MineOverlord.h"

#include "MineDefs.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

namespace acme {
///
/// \param minion
void MineOverlord::attach(MineMinion* minion) {
    _minions.push_back(minion);
}

///
int MineOverlord::getNumMinions() const {
    return static_cast<int>(_minions.size());
}

///
/// \param timestamp
void MineOverlord::notify(const std::string& timestamp) {
    for (auto* minion : _minions) {
        minion->update(timestamp);
    }
}

/// Runs through a simulation 'day' (72 hours)
void MineOverlord::run() {
    for (auto tick = 0; tick < TICKS_PER_DAY; ++tick) {
        auto timestamp = tickToTimestamp(tick);
        notify(timestamp);

        // 0.25s sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

/// Converts current tick to an HH:MM:SS timestamp
/// \param tick
/// \return
std::string MineOverlord::tickToTimestamp(int tick) {
    auto numSeconds = tick * SECONDS_PER_TICK;

    auto hours = numSeconds / 3600;
    numSeconds %= 3600;

    auto minutes = numSeconds / 60;
    auto seconds = numSeconds % 60;

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << hours << ":" << std::setw(2) << std::setfill('0')
        << minutes << ":" << std::setw(2) << std::setfill('0') << seconds;
    return oss.str();
}
}  // namespace acme