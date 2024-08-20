/// \file   MineStation.h
#pragma once
#include "MineOverlord.h"
#include "MineStationState.h"

#include <queue>
#include <string>

namespace acme {
class MineTruck;

/// \class  MineStation
class MineStation : public MineMinion {
public:
    ///
    explicit MineStation(const char* name);

    MineStation() = delete;
    ~MineStation() override = default;

    ///
    MineTruck* dequeue();

    ///
    void enqueue(MineTruck*);

    ///
    MineTruck* front();

    ///
    const char* getName() const override;

    ///
    std::size_t getQueueSize() const;

    ///
    void setStationState(StationState);

    ///
    void update(const std::string& timestamp) override;

private:
    std::string _stationName;
    std::string _timestamp;
    StationStateMap _stationStates;

    MineStationState* _currentState{nullptr};
    std::queue<MineTruck*> _truckQueue;
};
}  // namespace acme