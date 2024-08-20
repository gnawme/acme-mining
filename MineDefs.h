/// \file   MineDefs
#pragma once

namespace acme {
///
constexpr int MINING_DAY = 72;  // 72-hour mining day

constexpr int TICK_DURATION = 5;  // 5 minute ticks
constexpr int SECONDS_PER_TICK = TICK_DURATION * 60;
constexpr int TICKS_PER_HOUR = 60 / TICK_DURATION;
constexpr int TICKS_PER_DAY = (MINING_DAY * 60) / TICK_DURATION;

constexpr int TRUCK_TRANSIT_TIME = 30 / TICK_DURATION;   // 30 minute transit
constexpr int TRUCK_UNLOADING_TIME = 5 / TICK_DURATION;  // 5 minute unloading time
}  // namespace acme