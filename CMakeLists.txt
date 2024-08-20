cmake_minimum_required(VERSION 3.5)
project(acme-mining)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Find GTest package
find_package(GTest REQUIRED)

# Define source files
set(SIM_SOURCE
        AcmeMinerSim.cpp
        AcmeMinerUtils.cpp
        AcmeMinerUtils.h
        MineDefs.h
        MineDispatchers.cpp
        MineDispatchers.h
        MineLogger.h
        MineOverlord.cpp
        MineOverlord.h
        MineSite.cpp
        MineSite.h
        MineStation.cpp
        MineStation.h
        MineStationState.cpp
        MineStationState.h
        MineTimer.h
        MineTruck.cpp
        MineTruck.h
        MineTruckStates.cpp
        MineTruckStates.h
)

set(TEST_SOURCE MineTimerTest.cpp)

# Create the main executable
add_executable(acme-mining ${SIM_SOURCE})

# Include directories (if needed)
target_include_directories(acme-mining PRIVATE ${CMAKE_SOURCE_DIR})

# Link libraries (if needed)
target_link_libraries(acme-mining)

# Create test executable
add_executable(acme-unit-tests ${TEST_SOURCE})

# Link GTest and pthread libraries to the test executable
target_link_libraries(acme-unit-tests GTest::GTest GTest::Main pthread)
