/// \file   MineSite.h
/// \brief  Represents an H3 mining site
#pragma once
#include "MineOverlord.h"

#include <memory>
#include <string>

namespace acme {
class MineTimer;
/// \class  MineSite
class MineSite : public MineMinion {
public:
    ///
    explicit MineSite(const std::string& name);

    MineSite() = delete;
    ~MineSite() override = default;

    ///
    int getMiningDuration();

    ///
    const char* getName() const override;

    ///
    void outputStatistics(const std::string& timestamp) override;

    ///
    void setMiningFlag(bool beingMined);

    ///
    void update(const std::string& timestamp) override;

private:
    static bool _initial;
    std::string _siteName;
    std::string _timestamp;
    std::unique_ptr<MineTimer> _timer;

    int _duration{0};
    bool _beingMined{false};
    int _miningCount{0};
    int _idleCount{0};
};
}  // namespace acme