/// \file   MineStationState.cpp
#include "MineStationState.h"

#include "MineDefs.h"
#include "MineDispatchers.h"
#include "MineLogger.h"
#include "MineStation.h"
#include "MineTruck.h"

#include <fstream>
#include <sstream>

namespace acme {
///
/// \param context
MineStationIdle::MineStationIdle(MineStation& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineStationIdle::enterState() {
    _duration = TICKS_PER_DAY;
}

///
StationState MineStationIdle::getNextState() const {
    return StationState::READY;
}

///
StationState MineStationIdle::getState() const {
    return StationState::IDLE;
}

/// Gets the text of the state name
const char* MineStationIdle::getStateName() const {
    return STATION_STATE_NAME[StationState::IDLE];
}

///
void MineStationIdle::outputStatistics(std::ofstream& stationOutput) {
    stationOutput << _timeInState;
}

/// Updates the state with the context
void MineStationIdle::update(const std::string& timestamp) {
    ++_timeInState;
    if (_context.getQueueSize() != 0) {
        _context.setStationState(getNextState());
    }
}

///
/// \param context
MineStationReady::MineStationReady(MineStation& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineStationReady::enterState() {
    _duration = TRUCK_TRANSIT_TIME;
}

///
StationState MineStationReady::getNextState() const {
    return StationState::UNLOADING;
}

///
StationState MineStationReady::getState() const {
    return StationState::READY;
}

/// Gets the text of the state name
const char* MineStationReady::getStateName() const {
    return STATION_STATE_NAME[StationState::READY];
}

///
void MineStationReady::outputStatistics(std::ofstream& stationOutput) {
    stationOutput << _timeInState;
}

/// Updates the state with the context
void MineStationReady::update(const std::string& timestamp) {
    ++_timeInState;

    bool queued =
        ((_context.getQueueSize() != 0)
         && (_context.front()->getTruckState() != TruckState::QUEUED));

    if (queued) {
        std::ostringstream oss;
        oss << timestamp << " : Station ";
        oss << _context.getName() << " READY     with " << _context.getQueueSize() << " in queue";
        MineLogger::getInstance().logMessage(oss.str());
    } else {
        _context.setStationState(getNextState());
    }
}

///
/// \param context
MineStationUnloading::MineStationUnloading(MineStation& context)
    : _context(context) {}

/// Sets up conditions when the state is entered
/// \param duration
void MineStationUnloading::enterState() {
    _duration = TRUCK_UNLOADING_TIME;
}

///
StationState MineStationUnloading::getNextState() const {
    if (_context.getQueueSize() == 0) {
        return StationState::IDLE;
    } else {
        return StationState::READY;
    }
}

///
StationState MineStationUnloading::getState() const {
    return StationState::UNLOADING;
}

/// Gets the text of the state name
const char* MineStationUnloading::getStateName() const {
    return STATION_STATE_NAME[StationState::UNLOADING];
}

///
void MineStationUnloading::outputStatistics(std::ofstream& stationOutput) {
    stationOutput << _timeInState;
}

/// Updates the state with the context
void MineStationUnloading::update(const std::string& timestamp) {
    ++_timeInState;
    --_duration;

    if (_duration == 0) {
        // Remove the MineTruck from the queue, and requeue the MineStation
        auto* mineTruck = _context.dequeue();
        auto stationDispatcher = MineRegistry::getInstance().getStationDispatcher();
        stationDispatcher->enqueue(&_context);

        std::ostringstream oss;
        oss << timestamp << " : Station ";
        oss << _context.getName() << " UNLOADING truck " << mineTruck->getName();
        MineLogger::getInstance().logMessage(oss.str());
        _context.setStationState(getNextState());
    }
}
}  // namespace acme
