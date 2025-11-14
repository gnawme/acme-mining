/// \file   AcmeMinerTest.cpp
/// \brief  Unit tests for various Mine constructs
#include "AcmeMinerUtils.h"
#include "MineDefs.h"
#include "MineDispatchers.h"
#include "MineSite.h"
#include "MineStationState.h"
#include "MineTimer.h"
#include "MineTruck.h"
#include "MineTruckStates.h"

#include <gtest/gtest.h>

#include <memory>


using namespace acme;

///
struct AcmeMinerTest : public ::testing::Test {
    ///
    void SetUp() override {
        myMineTimer = std::make_unique<MineTimer>(H3_MINING_MIN, H3_MINING_MAX);

        myMineTruckA = std::make_unique<MineTruck>("ATRK-00000A");
        myMineTruckB = std::make_unique<MineTruck>("ATRK-00000B");
        myMineTruckC = std::make_unique<MineTruck>("ATRK-00000C");

        myMineStation1 = std::make_unique<MineStation>("ASTN-000001");
        myMineStation2 = std::make_unique<MineStation>("ASTN-000002");

        myMineSiteA = std::make_unique<MineSite>("ASIT-00000A");
        myMineSiteB = std::make_unique<MineSite>("ASIT-00000B");
        myMineSiteC = std::make_unique<MineSite>("ASIT-00000C");
    }

    ///
    void TearDown() override {}

    std::unique_ptr<MineTimer> myMineTimer;

    std::unique_ptr<MineTruck> myMineTruckA;
    std::unique_ptr<MineTruck> myMineTruckB;
    std::unique_ptr<MineTruck> myMineTruckC;

    std::unique_ptr<MineStation> myMineStation1;
    std::unique_ptr<MineStation> myMineStation2;

    std::unique_ptr<MineSite> myMineSiteA;
    std::unique_ptr<MineSite> myMineSiteB;
    std::unique_ptr<MineSite> myMineSiteC;
};

/// Tests MineTimer functionality
/// \note   Will sometimes fail, because two random numbers will randomly be the same
TEST_F(AcmeMinerTest, InstantiatingMineTimerWithDefaultValuesShouldWork) {
    auto* mineTimer = myMineTimer.get();
    auto duration1 = (*mineTimer)();
    EXPECT_TRUE(duration1 >= H3_MINING_MIN && duration1 <= H3_MINING_MAX);

    auto duration2 = (*mineTimer)();
    EXPECT_TRUE(duration2 >= H3_MINING_MIN && duration2 <= H3_MINING_MAX);
    EXPECT_NE(duration1, duration2);

    duration1 = (*mineTimer)();
    EXPECT_TRUE(duration1 >= H3_MINING_MIN && duration1 <= H3_MINING_MAX);
    for (auto i = 0; i < 10; ++i) {
        duration2 = (*mineTimer)();
        EXPECT_TRUE(duration2 >= H3_MINING_MIN && duration2 <= H3_MINING_MAX);
        EXPECT_NE(duration1, duration2);
        duration1 = duration2;
    }
}

/// Tests that the MineStation min heap works properly
TEST_F(AcmeMinerTest, StationDispatcherQueuesShouldWorkAsExpected) {
    auto* mineStation1 = myMineStation1.get();
    mineStation1->enqueue(myMineTruckA.get());
    mineStation1->enqueue(myMineTruckB.get());

    auto* mineStation2 = myMineStation2.get();
    mineStation2->enqueue(myMineTruckC.get());

    auto stationDispatcher = MineRegistry::getInstance().getStationDispatcher();
    stationDispatcher->enqueue(mineStation1);
    stationDispatcher->enqueue(mineStation2);

    // Station2 has the smaller queue, should be the one returned
    auto* availableStation = stationDispatcher->getNextAvailableStation();
    EXPECT_EQ(mineStation2->getName(), availableStation->getName());

    auto* truckA = mineStation1->dequeue();
    stationDispatcher->enqueue(mineStation1);

    mineStation2->enqueue(truckA);
    stationDispatcher->enqueue(mineStation1);

    // Station 1 now has the smaller queue, should be the one returned
    availableStation = stationDispatcher->getNextAvailableStation();
    EXPECT_EQ(mineStation1->getName(), availableStation->getName());
}

/// Tests that the MineSite queue works properly
TEST_F(AcmeMinerTest, SiteDispatcherShouldWorkAsExpected) {
    auto* mineSiteA = myMineSiteA.get();
    auto duration = mineSiteA->getMiningDuration();
    EXPECT_TRUE(duration >= H3_MINING_MIN && duration <= H3_MINING_MAX);
    auto siteDispatcher = MineRegistry::getInstance().getSiteDispatcher();
    siteDispatcher->enqueue(mineSiteA);

    auto* mineSiteB = myMineSiteB.get();
    siteDispatcher->enqueue(mineSiteB);

    auto* mineSiteC = myMineSiteC.get();
    siteDispatcher->enqueue(mineSiteC);

    auto* mineSiteAvailA = siteDispatcher->getNextAvailableMine();
    auto* mineSiteAvailB = siteDispatcher->getNextAvailableMine();
    siteDispatcher->enqueue(mineSiteAvailA);
    siteDispatcher->enqueue(mineSiteAvailB);

    auto* mineSiteAvailC = siteDispatcher->getNextAvailableMine();
    EXPECT_EQ(mineSiteAvailC->getName(), mineSiteC->getName());
}

///
TEST_F(AcmeMinerTest, MineStationStateTransitionsShouldWorkAsExpected) {
    auto* mineStation1 = myMineStation1.get();
    EXPECT_EQ(mineStation1->getState(), StationState::IDLE);
    auto stationDispatcher = MineRegistry::getInstance().getStationDispatcher();
    stationDispatcher->enqueue(mineStation1);

    // Place a MineTruck in the MineStation queue
    auto* mineTruckA = myMineTruckA.get();
    mineTruckA->assignMineSite(myMineSiteA.get());
    mineTruckA->setTruckState(TruckState::INBOUND);
    EXPECT_TRUE(mineStation1->getQueueSize() != 0);

    auto tick = 1;
    std::string timestamp = tickToTimestamp(tick);
    mineTruckA->update(timestamp);
    mineStation1->update(timestamp);
    EXPECT_EQ(mineStation1->getState(), StationState::READY);

    for (auto i = 1; i < TRUCK_TRANSIT_TIME; ++i) {
        ++tick;
        timestamp = tickToTimestamp(tick);
        mineTruckA->update(timestamp);
        mineStation1->update(timestamp);
    }

    EXPECT_EQ(mineStation1->getState(), StationState::UNLOADING);

    ++tick;
    timestamp = tickToTimestamp(tick);
    mineTruckA->update(timestamp);
    mineStation1->update(timestamp);
    EXPECT_EQ(mineStation1->getState(), StationState::READY);
}