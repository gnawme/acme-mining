/// \file   MineOverlord.cpp
/// \brief  Clock publisher for all Mine constructs
#pragma once
#include <string>
#include <vector>

namespace acme {
/// \class  MineMinion
/// \brief  Observer for MineOverlord
class MineMinion {
public:
    ///
    virtual ~MineMinion() = default;

    ///
    virtual const char* getName() const = 0;

    ///
    virtual void outputStatistics(const std::string& timestamp) = 0;

    ///
    virtual void update(const std::string& timestamp) = 0;
};

/// \class  MineOverlord
/// \brief  Subject (Publisher) of simulation timestamps
class MineOverlord {
public:
    ///
    void attach(MineMinion* minion);

    ///
    int getNumMinions() const;

    ///
    void notify(const std::string& timestamp);

    ///
    void outputStatistics();

    ///
    void run();

private:
    std::vector<MineMinion*> _minions;
};
}  // namespace acme