/// \file   AcmeMinerUtils.cpp
#include "AcmeMinerUtils.h"
#include "MineDispatchers.h"
#include "MineOverlord.h"
#include "MineSite.h"
#include "MineTruck.h"

#include <iomanip>
#include <sstream>

namespace acme {
/// Generates a standard name for a MineMinion
/// \param prefix
/// \param serial
/// \return
const char* genMinionName(const char* prefix, int serial) {
    std::stringstream ss;
    ss << prefix << "-" << std::setw(6) << std::setfill('0') << serial;
    return ss.str().c_str();
}

/// Instantiates all MineSite instances and attaches them as Observers
/// \param overlord
/// \param numSites
void instantiateSites(MineOverlord& overlord, int numSites) {
    auto siteDispatcher = MineRegistry::getInstance().getSiteDispatcher();
    for (auto site = 0; site < numSites; ++site) {
        constexpr char SITE_PREFIX[]{"ASIT"};
        auto name = genMinionName(SITE_PREFIX, site);
        auto miningSite = new MineSite(name);
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
        auto miningStation = new MineStation(name);
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
        auto miningTruck = new MineTruck(name);
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
}  // namespace acme