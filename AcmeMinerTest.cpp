/// \file   AcmeMinerTest.cpp
/// \brief  Unit tests for various Mine constructs
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

        myMineTruckA = new MineTruck("TRUCK_A");
        myMineTruckB = new MineTruck("TRUCK_B");
        myMineTruckC = new MineTruck("TRUCK_C");

        myMineStation1 = new MineStation("STATION_1");
        myMineStation2 = new MineStation("STATION_2");

        myMineSiteA = new MineSite("SITE_A");
        myMineSiteB = new MineSite("SITE_A");
        myMineSiteC = new MineSite("SITE_A");
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

///
TEST_F(AcmeMinerTest, InstantiatingMineTimerWithDefaultValuesShouldWork) {
    auto duration1 = (*myMineTimer)();
    auto duration2 = (*myMineTimer)();
    EXPECT_NE(duration1, duration2);
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