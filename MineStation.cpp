/// \file   MineStation.cpp
#include "MineStation.h"

namespace acme {
///
/// \param name
MineStation::MineStation(const char* name)
    : _stationName(name) {}

///
std::size_t MineStation::getQueueSize() const {
    return _myQueue.size();
}

///
const char* MineStation::getName() const {
    return _stationName.c_str();
}

///
/// \param timestamp
void MineStation::update(const std::string& timestamp) {
    _timestamp = timestamp;
};
}  // namespace acme