/// \file   MineTruck.cpp
#include "MineTruck.h"

#include <fstream>
#include <iostream>
#include <memory>

namespace acme {
bool MineTruck::_initial = true;
bool MineTruck::_revisited = true;

///
/// \param name
MineTruck::MineTruck(const std::string& name)
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
std::string MineTruck::getName() const {
    return _truckName;
}

///
int MineTruck::getPlaceInQueue() const {
    return _placeInQueue;
}

TruckState MineTruck::getTruckState() const {
    return _currentState->getState();
}

/// Outputs stations visited for this MineTruck
void MineTruck::outputStationVisits(const std::string& timestamp) {
    std::string mineTruckOutput(timestamp + "_StationVisits" + ".csv");
    std::ofstream truckOutput(mineTruckOutput, std::ios::app);

    if (MineTruck::_revisited) {
        truckOutput << "Truck,Site,Visits" << std::endl;
        MineTruck::_revisited = false;
    }

    auto truckState = _truckStates[TruckState::INBOUND];
    auto inbound = static_cast<MineTruckInbound*>(truckState.get());
    inbound->outputStationVisits(truckOutput);
}

/// Outputs MineTruck stats; delegates to MineTruckState classes
/// \param timestamp
void MineTruck::outputStatistics(const std::string& timestamp) {
    std::string mineTruckOutput(timestamp + "_MineTruck" + ".csv");
    std::ofstream truckOutput(mineTruckOutput, std::ios::app);

    if (MineTruck::_initial) {
        truckOutput << "Truck,Mining,Inbound,Queued,Unloading,Outbound" << std::endl;
        MineTruck::_initial = false;
    }

    truckOutput << getName() << ",";
    _truckStates[TruckState::MINING]->outputStatistics(truckOutput);
    truckOutput << ",";
    _truckStates[TruckState::INBOUND]->outputStatistics(truckOutput);
    truckOutput << ",";
    _truckStates[TruckState::QUEUED]->outputStatistics(truckOutput);
    truckOutput << ",";
    _truckStates[TruckState::UNLOADING]->outputStatistics(truckOutput);
    truckOutput << ",";
    _truckStates[TruckState::OUTBOUND]->outputStatistics(truckOutput);
    truckOutput << std::endl;
}

///
/// \param placeInQueue
void MineTruck::setPlaceInQueue(int placeInQueue) {
    _placeInQueue = placeInQueue;
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