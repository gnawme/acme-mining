/// \file   MineTimerTest.cpp
#include "MineTimer.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace acme;

///
struct MineTimerTest : public ::testing::Test {
    void SetUp() override {}
};

///
TEST_F(MineTimerTest, InstantiatingWithDefaultValuesShouldWork) {
    MineTimer myMineTimer(H3_MINING_MIN, H3_MINING_MAX);
    auto duration1 = myMineTimer();
    auto duration2 = myMineTimer();
    EXPECT_NE(duration1, duration2);
}