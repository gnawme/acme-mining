/// \file   MineStation.cpp
#include "MineStation.h"

#include <fstream>

namespace acme {
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
    auto mineTruck = _truckQueue.front();
    _truckQueue.pop();
    return mineTruck;
}

///
/// \param mineTruck
void MineStation::enqueue(MineTruck* mineTruck) {
    _truckQueue.push(mineTruck);
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
/// \param timestamp
void MineStation::outputStatistics(const std::string& timestamp) {
    std::string mineStationOutput("MineStation_" + timestamp + ".csv");
    std::ofstream stationOutput(mineStationOutput, std::ios::app);

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
};
}  // namespace acme