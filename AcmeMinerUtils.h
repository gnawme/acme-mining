/// \file   AcmeMinerUtils
#pragma once

namespace acme {
class MineOverlord;
///
const char* genMinionName(const char* prefix, int serial);

///
void instantiateSites(MineOverlord& overlord, int numSites);

///
void instantiateStations(MineOverlord& overlord, int numStations);

///
void instantiateTrucks(MineOverlord& overlord, int numTrucks);

///
void startTrucksAtMines();
}  // namespace acme