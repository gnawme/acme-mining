/// \file   MineTruck.h
#pragma once
#include "MineOverlord.h"
#include "MineTruckStates.h"

#include <string>

namespace acme {
class MineSite;
class MineStation;

/// \class  MineTruck
class MineTruck : public MineMinion {
public:
    ///
    explicit MineTruck(const char* name);

    MineTruck() = delete;
    ~MineTruck() override = default;

    ///
    void assignMineSite(MineSite*);

    ///
    void assignMineStation(MineStation*);

    ///
    MineSite* getAssignedMineSite() const;

    ///
    MineStation* getAssignedMineStation() const;

    ///
    const char* getName() const override;

    ///
    TruckState getTruckState() const;

    ///
    void outputStatistics(const std::string& timestamp) override;

    ///
    void setTruckState(TruckState);

    ///
    void update(const std::string& timestamp) override;

private:
    std::string _truckName;
    std::string _timestamp;
    TruckStateMap _truckStates;

    MineTruckState* _currentState{nullptr};
    MineSite* _mineSite{nullptr};
    MineStation* _mineStation{nullptr};
};
}  // namespace acme