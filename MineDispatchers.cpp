/// \file   MineDispatchers.cpp
#include "MineDispatchers.h"

#include "MineSite.h"

namespace acme {
/// Pushes a MineSite onto the (idle) queue
/// \param mineSite
void SiteDispatcher::enqueue(MineSite* mineSite) {
    _siteQueue.push(mineSite);
}

/// Gets an idle MineSite from the front of the queue
MineSite* SiteDispatcher::getNextAvailableMine() {
    auto mineSite = _siteQueue.front();
    _siteQueue.pop();
    return mineSite;
}

/// Pushes a MineStation and resorts the priority queue
/// \param mineStation
void StationDispatcher::enqueue(MineStation* mineStation) {
    _stationQueue.push(mineStation);
}

/// Gets the MineStation with the shortest wait from the front of the queue
MineStation* StationDispatcher::getNextAvailableStation() {
    auto mineStation = _stationQueue.top();
    _stationQueue.pop();
    return mineStation;
}
}  // namespace acme