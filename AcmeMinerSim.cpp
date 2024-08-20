/// \file   AcmeMinerSim.cpp
#include "AcmeMinerUtils.h"
#include "MineOverlord.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <string>

using namespace acme;

///
int main(int argc, char** argv) {
    // Usage note if incorrect number of arguments
    if (argc != 3) {
        std::cerr << "Usage: acme-mining <number-of-trucks> <number-of-stations>" << std::endl;
        EXIT_FAILURE;
    }

    // Process input
    auto numTrucks = std::stoi(*++argv);
    assert(numTrucks > 0 && numTrucks < std::numeric_limits<int>::max());

    auto numStations = std::stoi(*++argv);
    assert(numStations > 0 && numStations < std::numeric_limits<int>::max());

    auto numSites = numTrucks;
    std::cout << "Setting up simulation with " << numTrucks << " trucks, " << numSites
              << " mining sites, and " << numStations << " stations." << std::endl;

    // Instantiate simulation objects
    auto overlord = MineOverlord();
    instantiateTrucks(overlord, numTrucks);
    instantiateStations(overlord, numStations);
    instantiateSites(overlord, numSites);

    // All trucks are at mines initially
    startTrucksAtMines();

    // Run one simulation day
    overlord.run();
    EXIT_SUCCESS;
}