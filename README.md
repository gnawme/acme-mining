# ACME Helium-3 Lunar Mining Operations Simulator

### Introduction

The ACME Helium-3 Lunar Mining Operations (AHLMO) Simulator models our fully autonomous mining operations, consisting of:
* autonomous mining trucks
* autonomous unloading stations

AHLMO supports N mining trucks and M unloading stations that carry out mining operations over the course of a 72-hour mining "day."

### Building AHLMO

AHLMO employs CMake, and supports the usual CMake build steps:
1. Create a `build` directory
2. Change directories into the `build` directory: `cd build`
3. Use CMake to configure the build: `cmake ..`
4. Use CMake or Make to build the applications: `cmake --build .` or `make`

Note that CMake assumes that Google Test is installed where CMake can find it.

### Running AHLMO

AHLMO supports a small suite of unit tests; run them by invoking

`acme-unit-tests`

Run the AHLMO simulator with this command:

`acme-mining N M`

where `N` is the number of trucks, and `M` is the number of unloading stations.

AHLMO will take about 3-1/2 minutes to simulate a 72-hour mining day, and will produce a log and several time-stamped `CSV` files suitable for further statistical analysis.
