/// \file   AcmeMinerTest.cpp
/// \brief  Unit tests for various Mine constructs
#include "AcmeMinerUtils.h"
#include "MineDispatchers.h"
#include "MineSite.h"
#include "MineTimer.h"
#include "MineTruck.h"

#include <gtest/gtest.h>

#include <iostream>
#include <memory>

using namespace acme;

///
struct AcmeMinerTest : public ::testing::Test {
    ///
    void SetUp() override {
        myMineTimer = new MineTimer(H3_MINING_MIN, H3_MINING_MAX);

        myMineTruckA = new MineTruck("ATRK-00000A");
        myMineTruckB = new MineTruck("ATRK-00000B");
        myMineTruckC = new MineTruck("ATRK-00000C");

        myMineStation1 = new MineStation("ASTN-000001");
        myMineStation2 = new MineStation("ASTN-000001");

        myMineSiteA = new MineSite("ASIT-00000A");
        myMineSiteB = new MineSite("ASIT-00000B");
        myMineSiteC = new MineSite("ASIT-00000C");
    }

    ///
    void TearDown() override {
        delete myMineTimer;

        delete myMineTruckA;
        delete myMineTruckB;
        delete myMineTruckC;

        delete myMineStation1;
        delete myMineStation2;

        delete myMineSiteA;
        delete myMineSiteB;
        delete myMineSiteC;
    }

    MineTimer* myMineTimer{nullptr};

    MineTruck* myMineTruckA{nullptr};
    MineTruck* myMineTruckB{nullptr};
    MineTruck* myMineTruckC{nullptr};

    MineStation* myMineStation1{nullptr};
    MineStation* myMineStation2{nullptr};

    MineSite* myMineSiteA{nullptr};
    MineSite* myMineSiteB{nullptr};
    MineSite* myMineSiteC{nullptr};
};

/// Tests MineTimer functionality
/// \note   Will sometimes fail, because two random numbers will randomly be the same
TEST_F(AcmeMinerTest, InstantiatingMineTimerWithDefaultValuesShouldWork) {
    auto duration1 = (*myMineTimer)();
    EXPECT_TRUE(duration1 >= H3_MINING_MIN && duration1 <= H3_MINING_MAX);

    auto duration2 = (*myMineTimer)();
    EXPECT_TRUE(duration2 >= H3_MINING_MIN && duration2 <= H3_MINING_MAX);
    EXPECT_NE(duration1, duration2);

    duration1 = (*myMineTimer)();
    EXPECT_TRUE(duration1 >= H3_MINING_MIN && duration1 <= H3_MINING_MAX);
    for (auto i = 0; i < 10; ++i) {
        duration2 = (*myMineTimer)();
        EXPECT_TRUE(duration2 >= H3_MINING_MIN && duration2 <= H3_MINING_MAX);
        EXPECT_NE(duration1, duration2);
        duration1 = duration2;
    }
}

/// Tests that the MineStation min heap works properly
TEST_F(AcmeMinerTest, StationDispatcherQueuesShouldWorkAsExpected) {
    myMineStation1->enqueue(myMineTruckA);
    myMineStation1->enqueue(myMineTruckB);

    myMineStation2->enqueue(myMineTruckC);

    auto stationDispatcher = MineRegistry::getInstance().getStationDispatcher();
    stationDispatcher->enqueue(myMineStation1);
    stationDispatcher->enqueue(myMineStation2);

    // Station2 has the smaller queue, should be the one returned
    auto* availableStation = stationDispatcher->getNextAvailableStation();
    EXPECT_EQ(myMineStation2->getName(), availableStation->getName());

    auto truckA = myMineStation1->dequeue();
    stationDispatcher->enqueue(myMineStation1);

    myMineStation2->enqueue(truckA);
    stationDispatcher->enqueue(myMineStation1);

    // Station 1 now has the smaller queue, should be the one returned
    availableStation = stationDispatcher->getNextAvailableStation();
    EXPECT_EQ(myMineStation1->getName(), availableStation->getName());
}

/// Tests that the MineSite queue works properly
TEST_F(AcmeMinerTest, SiteDispatcherShouldWorkAsExpected) {
    auto duration = myMineSiteA->getMiningDuration();
    EXPECT_TRUE(duration >= H3_MINING_MIN && duration <= H3_MINING_MAX);
    auto siteDispatcher = MineRegistry::getInstance().getSiteDispatcher();
    siteDispatcher->enqueue(myMineSiteA);
    siteDispatcher->enqueue(myMineSiteB);
    siteDispatcher->enqueue(myMineSiteC);

    auto* mineSiteA = siteDispatcher->getNextAvailableMine();
    auto* mineSiteB = siteDispatcher->getNextAvailableMine();
    siteDispatcher->enqueue(mineSiteA);
    siteDispatcher->enqueue(mineSiteB);

    auto* mineSiteC = siteDispatcher->getNextAvailableMine();
    EXPECT_EQ(mineSiteC->getName(), myMineSiteC->getName());
}

///
TEST_F(AcmeMinerTest, MineStationStateTransitionsShouldWorkAsExpected) {
    EXPECT_EQ(myMineStation1->getState(), StationState::IDLE);
    auto stationDispatcher = MineRegistry::getInstance().getStationDispatcher();
    stationDispatcher->enqueue(myMineStation1);

    // Place a MineTruck in the MineStation queue
    myMineTruckA->assignMineSite(myMineSiteA);
    myMineTruckA->setTruckState(TruckState::INBOUND);
    EXPECT_TRUE(myMineStation1->getQueueSize() != 0);

    auto tick = 1;
    std::string timestamp = tickToTimestamp(tick);
    myMineTruckA->update(timestamp);
    myMineStation1->update(timestamp);
    EXPECT_EQ(myMineStation1->getState(), StationState::READY);

    for (auto i = 1; i < TRUCK_TRANSIT_TIME; ++i) {
        ++tick;
        timestamp = tickToTimestamp(tick);
        myMineTruckA->update(timestamp);
        myMineStation1->update(timestamp);
    }

    EXPECT_EQ(myMineStation1->getState(), StationState::UNLOADING);

    ++tick;
    timestamp = tickToTimestamp(tick);
    myMineTruckA->update(timestamp);
    myMineStation1->update(timestamp);
    EXPECT_EQ(myMineStation1->getState(), StationState::READY);
}