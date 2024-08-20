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
    _duration = mineSite->getMiningDuration();
}

///
TruckState MineTruckMining::getNextState() const {
    return TruckState::INBOUND;
}

/// Gets the text of the state name
const char* MineTruckMining::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::MINING];
}

/// Updates the state with the context
void MineTruckMining::update(const std::string& timestamp) {
    if (_duration % 5 == 0 || _duration < 5) {
        std::ostringstream oss;
        oss << timestamp << " : Truck ";
        oss << _context.getName() << " MINING at " << _context.getAssignedMineSite()->getName();
        oss << ", remaining duration " << (_duration * TICK_DURATION) << " minutes";
        MineLogger::getInstance().logMessage(oss.str());
    }
    ++_timeInState;
    --_duration;

    if (_duration == 0) {
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
    auto* mineStation =
        MineRegistry::getInstance().getStationDispatcher()->getNextAvailableStation();
    _context.assignMineStation(mineStation);
}

///
TruckState MineTruckInbound::getNextState() const {
    return TruckState::QUEUED;
}

/// Gets the text of the state name
const char* MineTruckInbound::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::INBOUND];
}

/// Updates the state with the context
void MineTruckInbound::update(const std::string& timestamp) {
    if (_duration % 5 == 0 || _duration < 5) {
        std::ostringstream oss;
        oss << timestamp << " : Truck ";
        oss << _context.getName() << " INBOUND to " << _context.getAssignedMineStation()->getName();
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
    auto* mineStation = _context.getAssignedMineStation();
    _duration = static_cast<int>(mineStation->getQueueSize() * TRUCK_UNLOADING_TIME);
}

///
TruckState MineTruckQueued::getNextState() const {
    return TruckState::UNLOADING;
}

/// Gets the text of the state name
const char* MineTruckQueued::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::QUEUED];
}

/// Updates the state with the context
void MineTruckQueued::update(const std::string& timestamp) {
    if (_duration == 0) {
        _context.setTruckState(getNextState());
    }

    std::ostringstream oss;
    oss << timestamp << " : Truck ";
    oss << _context.getName() << " QUEUED at " << _context.getAssignedMineStation()->getName();
    oss << ", estimated wait time " << (_duration * TICK_DURATION) << " minutes";
    MineLogger::getInstance().logMessage(oss.str());

    ++_timeInState;
    --_duration;
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
TruckState MineTruckUnloading::getNextState() const {
    return TruckState::OUTBOUND;
}

/// Gets the text of the state name
const char* MineTruckUnloading::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::UNLOADING];
}

/// Updates the state with the context
void MineTruckUnloading::update(const std::string& timestamp) {
    std::ostringstream oss;
    oss << timestamp << " : Truck ";
    oss << _context.getName() << " UNLOADING at  " << _context.getAssignedMineStation()->getName();
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
TruckState MineTruckOutbound::getNextState() const {
    return TruckState::MINING;
}

/// Gets the text of the state name
const char* MineTruckOutbound::getStateName() const {
    return TRUCK_STATE_NAME[TruckState::OUTBOUND];
}

/// Updates the state with the context
void MineTruckOutbound::update(const std::string& timestamp) {
    if (_duration % 5 == 0 || _duration < 5) {
        std::ostringstream oss;
        oss << timestamp << " : Truck ";
        oss << _context.getName() << " OUTBOUND to " << _context.getAssignedMineSite()->getName();
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