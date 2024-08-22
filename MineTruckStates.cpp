/// \file   MineTruckStates.cpp
#include "MineTruckStates.h"

#include "MineDefs.h"
#include "MineDispatchers.h"
#include "MineLogger.h"
#include "MineSite.h"
#include "MineStation.h"
#include "MineTruck.h"

#include <sstream>

namespace acme {
///
/// \param context
MineTruckMining::MineTruckMining(MineTruck& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineTruckMining::enterState() {
    auto* mineSite = _context.getAssignedMineSite();
    mineSite->setMiningFlag(true);
    _duration = mineSite->getMiningDuration();
}

///
TruckState MineTruckMining::getState() const {
    return TruckState::MINING;
}

///
TruckState MineTruckMining::getNextState() const {
    return TruckState::INBOUND;
}

/// Gets the text of the state name
const char* MineTruckMining::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::MINING];
}

///
void MineTruckMining::outputStatistics(std::ofstream& truckOutput) {
    truckOutput << _timeInState;
}

/// Updates the state with the context
void MineTruckMining::update(const std::string& timestamp) {
    if (_duration % 5 == 0 || _duration < 10) {
        std::ostringstream oss;
        oss << timestamp << " : Truck   ";
        oss << _context.getName() << " MINING    at " << _context.getAssignedMineSite()->getName();
        oss << ", remaining duration " << (_duration * TICK_DURATION) << " minutes";
        MineLogger::getInstance().logMessage(oss.str());
    }
    ++_timeInState;
    --_duration;

    if (_duration == 0) {
        // Place MineSite on available queue
        MineRegistry::getInstance().getSiteDispatcher()->enqueue(_context.getAssignedMineSite());
        _context.setTruckState(getNextState());
    }
}

///
/// \param context
MineTruckInbound::MineTruckInbound(MineTruck& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineTruckInbound::enterState() {
    _duration = TRUCK_TRANSIT_TIME;

    // Get the MineStation with the shortest queue and place this MineTruck on its queue
    auto* mineStation =
        MineRegistry::getInstance().getStationDispatcher()->getNextAvailableStation();
    _context.assignMineStation(mineStation);
    auto placeInQueue = mineStation->enqueue(&_context);
    _context.setPlaceInQueue(placeInQueue);
    MineRegistry::getInstance().getStationDispatcher()->enqueue(mineStation);

    // No longer mining
    _context.getAssignedMineSite()->setMiningFlag(false);
}

///
TruckState MineTruckInbound::getState() const {
    return TruckState::INBOUND;
}

///
TruckState MineTruckInbound::getNextState() const {
    return TruckState::QUEUED;
}

/// Gets the text of the state name
const char* MineTruckInbound::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::INBOUND];
}

///
void MineTruckInbound::outputStatistics(std::ofstream& truckOutput) {
    truckOutput << _timeInState;
}

/// Updates the state with the context
void MineTruckInbound::update(const std::string& timestamp) {
    if (_duration % 3 == 0) {
        std::ostringstream oss;
        oss << timestamp << " : Truck   ";
        oss << _context.getName() << " INBOUND   to "
            << _context.getAssignedMineStation()->getName();
        oss << ", remaining duration " << (_duration * TICK_DURATION) << " minutes";
        MineLogger::getInstance().logMessage(oss.str());
    }

    ++_timeInState;
    --_duration;

    if (_duration == 0) {
        _context.setTruckState(getNextState());
    }
}

///
/// \param context
MineTruckQueued::MineTruckQueued(MineTruck& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineTruckQueued::enterState() {
    _duration = _context.getPlaceInQueue() * TRUCK_UNLOADING_TIME;
}

///
TruckState MineTruckQueued::getState() const {
    return TruckState::QUEUED;
}

///
TruckState MineTruckQueued::getNextState() const {
    return TruckState::UNLOADING;
}

/// Gets the text of the state name
const char* MineTruckQueued::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::QUEUED];
}

///
void MineTruckQueued::outputStatistics(std::ofstream& truckOutput) {
    truckOutput << _timeInState;
}

/// Updates the state with the context
void MineTruckQueued::update(const std::string& timestamp) {
    auto* mineStation = _context.getAssignedMineStation();
    std::ostringstream oss;
    oss << timestamp << " : Truck   ";
    oss << _context.getName() << " QUEUED    at " << mineStation->getName();
    oss << ", estimated wait time " << (_duration * TICK_DURATION) << " minutes";
    MineLogger::getInstance().logMessage(oss.str());

    ++_timeInState;
    --_duration;

    if (_duration == 0) {
        // Remove the MineTruck from the queue, and place the MineStation back in the dispatcher
        // queue
        mineStation->dequeue();
        auto stationDispatcher = MineRegistry::getInstance().getStationDispatcher();
        stationDispatcher->enqueue(mineStation);

        _context.setTruckState(getNextState());
    }
}

///
/// \param context
MineTruckUnloading::MineTruckUnloading(MineTruck& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineTruckUnloading::enterState() {
    _duration = TRUCK_UNLOADING_TIME;
}

///
TruckState MineTruckUnloading::getState() const {
    return TruckState::UNLOADING;
}

///
TruckState MineTruckUnloading::getNextState() const {
    return TruckState::OUTBOUND;
}

/// Gets the text of the state name
const char* MineTruckUnloading::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::UNLOADING];
}

///
void MineTruckUnloading::outputStatistics(std::ofstream& truckOutput) {
    truckOutput << _timeInState;
}

/// Updates the state with the context
void MineTruckUnloading::update(const std::string& timestamp) {
    std::ostringstream oss;
    oss << timestamp << " : Truck   ";
    oss << _context.getName() << " UNLOADING at " << _context.getAssignedMineStation()->getName();
    oss << ", duration 5 minutes";
    MineLogger::getInstance().logMessage(oss.str());

    ++_timeInState;
    --_duration;

    if (_duration == 0) {
        _context.setTruckState(getNextState());
    }
}

///
/// \param context
MineTruckOutbound::MineTruckOutbound(MineTruck& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineTruckOutbound::enterState() {
    auto* mineSite = MineRegistry::getInstance().getSiteDispatcher()->getNextAvailableMine();
    _context.assignMineSite(mineSite);
    _duration = TRUCK_TRANSIT_TIME;
}

///
TruckState MineTruckOutbound::getState() const {
    return TruckState::OUTBOUND;
}

///
TruckState MineTruckOutbound::getNextState() const {
    return TruckState::MINING;
}

/// Gets the text of the state name
const char* MineTruckOutbound::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::OUTBOUND];
}

///
void MineTruckOutbound::outputStatistics(std::ofstream& truckOutput) {
    truckOutput << _timeInState;
}

/// Updates the state with the context
void MineTruckOutbound::update(const std::string& timestamp) {
    if (_duration % 5 == 0 || _duration < 10) {
        std::ostringstream oss;
        oss << timestamp << " : Truck   ";
        oss << _context.getName() << " OUTBOUND  to " << _context.getAssignedMineSite()->getName();
        oss << ", remaining duration " << (_duration * TICK_DURATION) << " minutes";
        MineLogger::getInstance().logMessage(oss.str());
    }

    ++_timeInState;
    --_duration;

    if (_duration == 0) {
        _context.setTruckState(getNextState());
    }
}
}  // namespace acme