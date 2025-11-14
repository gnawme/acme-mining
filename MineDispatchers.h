/// \file   MineDispatchers.cpp
/// \brief  Dispatcher classes and their object registry
#pragma once
#include "MineStation.h"
#include "MineTruck.h"

#include <memory>
#include <queue>

namespace acme {
class MineSite;
///
class SiteDispatcher {
public:
    SiteDispatcher() = default;
    ~SiteDispatcher();

    ///
    void enqueue(MineSite* mineSite);

    ///
    MineSite* getNextAvailableMine();

    std::vector<std::unique_ptr<MineSite>> siteRegistry;

private:
    std::queue<MineSite*> _siteQueue;
};

/// Custom comparator for StationDispatcher; makes the priority_queue a min-heap
class StationDispatcher;
struct CompareQueueSize {
    bool operator()(const MineStation* station1, const MineStation* station2) {
        return station1->getQueueSize() > station2->getQueueSize();
    }
};

///
class StationDispatcher {
public:
    StationDispatcher() = default;
    ~StationDispatcher();

    ///
    void enqueue(MineStation*);

    ///
    MineStation* getNextAvailableStation();

    std::vector<std::unique_ptr<MineStation>> stationDepot;

private:
    std::priority_queue<MineStation*, std::vector<MineStation*>, CompareQueueSize> _stationQueue;
};

///
class TruckDispatcher {
public:
    TruckDispatcher() = default;
    ~TruckDispatcher();


    /// \note   Allows direct application access
    std::vector<std::unique_ptr<MineTruck>> truckGarage;
};

///
/// \brief  Registry for accessing Dispatchers
class MineRegistry {
public:
    ///
    static MineRegistry& getInstance() {
        static MineRegistry instance;
        return instance;
    }

    MineRegistry(const MineRegistry&) = delete;
    MineRegistry& operator=(const MineRegistry&) = delete;

    ///
    std::shared_ptr<SiteDispatcher> getSiteDispatcher();

    ///
    std::shared_ptr<StationDispatcher> getStationDispatcher();

    ///
    std::shared_ptr<TruckDispatcher> getTruckDispatcher();

private:
    MineRegistry() = default;
    ~MineRegistry() = default;

    std::shared_ptr<SiteDispatcher> _siteDispatcher;
    std::shared_ptr<StationDispatcher> _stationDispatcher;
    std::shared_ptr<TruckDispatcher> _truckDispatcher;
};
}  // namespace acme