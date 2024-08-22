/// \file   MineStation.cpp
#include "MineStation.h"

#include "MineTruck.h"

#include <fstream>
#include <iostream>

namespace acme {
bool MineStation::_initial = true;

///
/// \param name
MineStation::MineStation(const char* name)
    : _stationName(name) {
    _stationStates[StationState::IDLE] = std::make_shared<MineStationIdle>(*this);
    _stationStates[StationState::READY] = std::make_shared<MineStationReady>(*this);
    _stationStates[StationState::UNLOADING] = std::make_shared<MineStationUnloading>(*this);

    // Initial state is IDLE
    _currentState = _stationStates[StationState::IDLE].get();
}

///
MineTruck* MineStation::dequeue() {
    auto* mineTruck = _truckQueue.front();
    _truckQueue.pop();
    --_placeInQueue;
    return mineTruck;
}

///
/// \param mineTruck
int MineStation::enqueue(MineTruck* mineTruck) {
    _truckQueue.push(mineTruck);
    ++_placeInQueue;
    return _placeInQueue;
}

///
MineTruck* MineStation::front() {
    return _truckQueue.front();
}

///
std::size_t MineStation::getQueueSize() const {
    return _truckQueue.size();
}

///
const char* MineStation::getName() const {
    return _stationName.c_str();
}

///
StationState MineStation::getState() const {
    return _currentState->getState();
}

///
/// \param timestamp
void MineStation::outputStatistics(const std::string& timestamp) {
    std::string mineStationOutput(timestamp + "_MineStation" + ".csv");
    std::ofstream stationOutput(mineStationOutput, std::ios::app);

    if (MineStation::_initial) {
        stationOutput << "Station,Idle,Ready,Unloading" << std::endl;
        MineStation::_initial = false;
    }

    stationOutput << getName() << ",";
    _stationStates[StationState::IDLE]->outputStatistics(stationOutput);
    stationOutput << ",";
    _stationStates[StationState::READY]->outputStatistics(stationOutput);
    stationOutput << ",";
    _stationStates[StationState::UNLOADING]->outputStatistics(stationOutput);
    stationOutput << std::endl;
}

///
/// \param truckState
void MineStation::setStationState(StationState truckState) {
    _currentState = _stationStates[truckState].get();
    _currentState->enterState();
}

///
/// \param timestamp
void MineStation::update(const std::string& timestamp) {
    _timestamp = timestamp;
    _currentState->update(_timestamp);
};
}  // namespace acme