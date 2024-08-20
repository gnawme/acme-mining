/// \file   MineTruck.cpp
#include "MineTruck.h"

#include <iostream>
#include <memory>

namespace acme {
///
/// \param name
MineTruck::MineTruck(const char* name)
    : _truckName(name) {
    // Instantiate MineTruckStates
    _truckStates[TruckState::MINING] = std::make_shared<MineTruckMining>(*this);
    _truckStates[TruckState::INBOUND] = std::make_shared<MineTruckInbound>(*this);
    _truckStates[TruckState::QUEUED] = std::make_shared<MineTruckQueued>(*this);
    _truckStates[TruckState::UNLOADING] = std::make_shared<MineTruckUnloading>(*this);
    _truckStates[TruckState::OUTBOUND] = std::make_shared<MineTruckOutbound>(*this);

    // Initial state is MINING
    _currentState = _truckStates[TruckState::MINING].get();
}

/// Assigns a MineSite
/// \param mineSite
void MineTruck::assignMineSite(MineSite* mineSite) {
    _mineSite = mineSite;
}

/// Assigns a MineStation
/// \param mineStation
void MineTruck::assignMineStation(MineStation* mineStation) {
    _mineStation = mineStation;
}

///
MineSite* MineTruck::getAssignedMineSite() const {
    return _mineSite;
}

///
MineStation* MineTruck::getAssignedMineStation() const {
    return _mineStation;
}

///
const char* MineTruck::getName() const {
    return _truckName.c_str();
}

TruckState MineTruck::getTruckState() const {
    return _currentState->getState();
}

///
/// \param truckState
void MineTruck::setTruckState(TruckState truckState) {
    _currentState = _truckStates[truckState].get();
    _currentState->enterState();
}

///
/// \param timestamp
void MineTruck::update(const std::string& timestamp) {
    _timestamp = timestamp;
    _currentState->update(_timestamp);
};
}  // namespace acme