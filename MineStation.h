/// \file   MineStation.h
#pragma once
#include "MineOverlord.h"

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
    const char* getName() const override;

    ///
    std::size_t getQueueSize() const;

    ///
    void update(const std::string& timestamp) override;

private:
    std::string _stationName;
    std::string _timestamp;
    std::queue<MineTruck*> _myQueue;
};
}  // namespace acme