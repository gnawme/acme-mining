/// \file   MineTimer.h
#pragma once
#include "MineDefs.h"

#include <chrono>
#include <iostream>
#include <random>

namespace acme {
static constexpr int H3_MINING_MIN = 1 * TICKS_PER_HOUR;  // 1 hour = 12 ticks
static constexpr int H3_MINING_MAX = 5 * TICKS_PER_HOUR;  // 5 hours = 60 ticks

/// Generates random mining times
class MineTimer {
public:
    ///
    MineTimer(int min, int max)
        : _generator(std::chrono::system_clock::now().time_since_epoch().count())
        , _distribution(min, max) {}

    ///
    int operator()() {
        return _distribution(_generator);
    }

    MineTimer() = delete;

private:
    std::mt19937 _generator;                           // Mersenne Twister RNG
    std::uniform_int_distribution<int> _distribution;  // Uniform distribution
};
}  // namespace acme