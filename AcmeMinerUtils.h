/// \file   AcmeMinerUtils
#pragma once
#include <string>

namespace acme {
class MineOverlord;
///
std::string createISODateStamp();

///
std::string genMinionName(const char* prefix, int serial);

///
void instantiateSites(MineOverlord& overlord, int numSites);

///
void instantiateStations(MineOverlord& overlord, int numStations);

///
void instantiateTrucks(MineOverlord& overlord, int numTrucks);

///
void startTrucksAtMines();

///
std::string tickToTimestamp(int tick);
}  // namespace acme