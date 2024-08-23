/// \file   MineOverlord.cpp
#include "MineOverlord.h"

#include "AcmeMinerUtils.h"
#include "MineDefs.h"
#include "MineDispatchers.h"
#include "MineLogger.h"
#include "MineTruck.h"

#include <chrono>
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

///
void MineOverlord::outputStatistics() {
    auto timestamp = createISODateStamp();
    for (auto* minion : _minions) {
        minion->outputStatistics(timestamp);
    }

    auto truckDispatcher = MineRegistry::getInstance().getTruckDispatcher();
    for (auto truck : truckDispatcher->truckGarage) {
        truck->outputStationVisits(timestamp);
    }
}

/// Runs through a simulation 'day' (72 hours)
void MineOverlord::run(int numTrucks, int numStations) {
    std::ostringstream oss;
    oss << "ACME Helium-3 Lunar Mining Operations : " << createISODateStamp() << " : ";
    oss << numTrucks << " mining trucks, " << numStations << " unloading stations";
    MineLogger::getInstance().logMessage(oss.str());
    std::string separator(oss.str().length(), '=');
    MineLogger::getInstance().logMessage(separator);

    for (auto tick = 0; tick < TICKS_PER_DAY; ++tick) {
        auto timestamp = tickToTimestamp(tick);
        notify(timestamp);

        // 0.25s sleep
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}
}  // namespace acme