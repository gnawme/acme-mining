/// \file   MineDispatchers.cpp
/// \brief  Dispatcher classes and their object registry
#pragma once
#include "MineStation.h"

#include <memory>
#include <queue>

namespace acme {
class MineSite;
class MineTruck;
///
class SiteDispatcher {
public:
    SiteDispatcher() = default;

    ///
    void enqueue(MineSite* mineSite);

    ///
    MineSite* getNextAvailableMine();

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

    ///
    void enqueue(MineStation*);

    ///
    MineStation* getNextAvailableStation();

private:
    std::priority_queue<MineStation*, std::vector<MineStation*>, CompareQueueSize> _stationQueue;
};

///
class TruckDispatcher {
public:
    TruckDispatcher() = default;

    /// \note   Allows direct application access
    std::vector<MineTruck*> truckGarage;
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
    std::shared_ptr<SiteDispatcher> getSiteDispatcher() {
        if (!_siteDispatcher) {
            _siteDispatcher = std::make_shared<SiteDispatcher>();
        }
        return _siteDispatcher;
    }

    ///
    std::shared_ptr<StationDispatcher> getStationDispatcher() {
        if (!_stationDispatcher) {
            _stationDispatcher = std::make_shared<StationDispatcher>();
        }
        return _stationDispatcher;
    }

    ///
    std::shared_ptr<TruckDispatcher> getTruckDispatcher() {
        if (!_truckDispatcher) {
            _truckDispatcher = std::make_shared<TruckDispatcher>();
        }
        return _truckDispatcher;
    }

private:
    MineRegistry() = default;
    ~MineRegistry() = default;

    std::shared_ptr<SiteDispatcher> _siteDispatcher;
    std::shared_ptr<StationDispatcher> _stationDispatcher;
    std::shared_ptr<TruckDispatcher> _truckDispatcher;
};
}  // namespace acme