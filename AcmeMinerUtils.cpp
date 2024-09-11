/// \file   AcmeMinerUtils.cpp
#include "AcmeMinerUtils.h"

#include "MineDefs.h"
#include "MineDispatchers.h"
#include "MineOverlord.h"
#include "MineSite.h"
#include "MineTruck.h"

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace acme {
/// Creates an ISO date stamp for stats output files
std::string createISODateStamp() {
    // Get the current time as a time_point
    auto now = std::chrono::system_clock::now();

    // Convert to a time_t, which represents the time in seconds
    std::time_t timeNow = std::chrono::system_clock::to_time_t(now);

    // Use an ostringstream to format the date stamp
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&timeNow), "%Y-%m-%dT%H-%M-%S");

    // Return the formatted string
    return oss.str();
}

/// Generates a standard name for a MineMinion
/// \param prefix
/// \param serial
/// \return
std::string genMinionName(const char* prefix, int serial) {
    std::ostringstream oss;
    oss << prefix << "-" << std::setw(6) << std::setfill('0') << serial;
    return oss.str();
}

/// Instantiates all MineSite instances and attaches them as Observers
/// \param overlord
/// \param numSites
void instantiateSites(MineOverlord& overlord, int numSites) {
    auto siteDispatcher = MineRegistry::getInstance().getSiteDispatcher();
    for (auto site = 0; site < numSites; ++site) {
        constexpr char SITE_PREFIX[]{"ASIT"};
        auto name = genMinionName(SITE_PREFIX, site);
        auto* miningSite = new MineSite(name);
        overlord.attach(miningSite);
        siteDispatcher->enqueue(miningSite);
    }
}

/// Instantiates all MineStation instances and attaches them as Observers
/// \param overlord
/// \param numStations
void instantiateStations(MineOverlord& overlord, int numStations) {
    auto stationDispatcher = MineRegistry::getInstance().getStationDispatcher();
    for (auto station = 0; station < numStations; ++station) {
        constexpr char STATION_PREFIX[]{"ASTN"};
        auto name = genMinionName(STATION_PREFIX, station);
        auto* miningStation = new MineStation(name);
        overlord.attach(miningStation);
        stationDispatcher->enqueue(miningStation);
    }
}

/// Instantiates all MineTruck instances and attaches them as Observers
/// \param overlord
/// \param numTrucks
void instantiateTrucks(MineOverlord& overlord, int numTrucks) {
    auto truckDispatcher = MineRegistry::getInstance().getTruckDispatcher();
    for (auto truck = 0; truck < numTrucks; ++truck) {
        constexpr char TRUCK_PREFIX[]{"ATRK"};
        auto name = genMinionName(TRUCK_PREFIX, truck);
        auto* miningTruck = new MineTruck(name);
        overlord.attach(miningTruck);
        truckDispatcher->truckGarage.push_back(miningTruck);
    }
}

/// Makes initial association of trucks with mines, sets initial truck state to MINING
void startTrucksAtMines() {
    auto siteDispatcher = MineRegistry::getInstance().getSiteDispatcher();
    auto truckDispatcher = MineRegistry::getInstance().getTruckDispatcher();

    for (auto* truck : truckDispatcher->truckGarage) {
        truck->assignMineSite(siteDispatcher->getNextAvailableMine());
        truck->setTruckState(TruckState::MINING);
    }
}

/// Converts current tick to an HH:MM:SS timestamp
/// \param tick
/// \return
std::string tickToTimestamp(int tick) {
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