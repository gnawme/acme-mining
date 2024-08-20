/// \file   MineStationState.cpp
#include "MineStationState.h"

#include "MineDefs.h"
#include "MineStation.h"
#include "MineTruck.h"

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

/// Gets the text of the state name
const char* MineStationIdle::getStateName() const {
    return STATION_STATE_NAME[StationState::IDLE];
}

/// Updates the state with the context
void MineStationIdle::update(const std::string& timestamp) {
    ++_timeInState;
    --_duration;

    if (_duration == 0) {
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

/// Gets the text of the state name
const char* MineStationReady::getStateName() const {
    return STATION_STATE_NAME[StationState::READY];
}

/// Updates the state with the context
void MineStationReady::update(const std::string& timestamp) {
    ++_timeInState;
    --_duration;

    bool queued =
        ((_context.getQueueSize() != 0)
         && (_context.front()->getTruckState() != TruckState::INBOUND));
    if (_duration == 0 || queued) {
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
    _duration = 0;
}

///
StationState MineStationUnloading::getNextState() const {
    if (_context.getQueueSize() == 0) {
        return StationState::IDLE;
    } else {
        return StationState::READY;
    }
}

/// Gets the text of the state name
const char* MineStationUnloading::getStateName() const {
    return STATION_STATE_NAME[StationState::UNLOADING];
}

/// Updates the state with the context
void MineStationUnloading::update(const std::string& timestamp) {
    ++_timeInState;
    --_duration;

    if (_duration == 0) {
        _context.dequeue();
        _context.setStationState(getNextState());
    }
}
}  // namespace acme
