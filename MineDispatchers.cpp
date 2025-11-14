/// \file   MineDispatchers.cpp
#include "MineDispatchers.h"

#include "MineSite.h"
#include "MineStation.h"

#include <cassert>

namespace acme {
///
std::shared_ptr<SiteDispatcher> MineRegistry::getSiteDispatcher() {
    if (!_siteDispatcher) {
        _siteDispatcher = std::make_shared<SiteDispatcher>();
    }
    return _siteDispatcher;
}

///
std::shared_ptr<StationDispatcher> MineRegistry::getStationDispatcher() {
    if (!_stationDispatcher) {
        _stationDispatcher = std::make_shared<StationDispatcher>();
    }
    return _stationDispatcher;
}

///
std::shared_ptr<TruckDispatcher> MineRegistry::getTruckDispatcher() {
    if (!_truckDispatcher) {
        _truckDispatcher = std::make_shared<TruckDispatcher>();
    }
    return _truckDispatcher;
}

///
SiteDispatcher::~SiteDispatcher() = default;

/// Pushes a MineSite onto the (idle) queue
/// \param mineSite
void SiteDispatcher::enqueue(MineSite* mineSite) {
    _siteQueue.push(mineSite);
}

/// Gets an idle MineSite from the front of the queue
MineSite* SiteDispatcher::getNextAvailableMine() {
    assert(!_siteQueue.empty());
    auto* mineSite = _siteQueue.front();
    _siteQueue.pop();
    return mineSite;
}

///
StationDispatcher::~StationDispatcher() = default;

/// Pushes a MineStation and resorts the priority queue
/// \param mineStation
void StationDispatcher::enqueue(MineStation* mineStation) {
    _stationQueue.push(mineStation);
}

/// Gets the MineStation with the shortest wait from the front of the queue
MineStation* StationDispatcher::getNextAvailableStation() {
    auto* mineStation = _stationQueue.top();
    _stationQueue.pop();
    return mineStation;
}


TruckDispatcher::~TruckDispatcher() = default;
}  // namespace acme