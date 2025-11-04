# ACME Helium-3 Lunar Mining Operations Simulator

![ACME Mining Truck](docs/ACME%20Mining%20Truck.png)

### Introduction

The ACME Helium-3 Lunar Mining Operations (AHLMO) Simulator models our fully autonomous mining operations, consisting of:
* autonomous mining trucks
* autonomous unloading stations

AHLMO supports N mining trucks and M unloading stations that carry out mining operations over the course of a 72-hour mining "day."

### Prerequisites

- CMake 3.15 or higher
- Conan 2.x package manager
- C++17 compatible compiler

### Building AHLMO

AHLMO uses CMake and Conan for dependency management. Conan automatically handles the Google Test dependency.

#### Option 1: Step-by-step build

1. Install dependencies with Conan:
   ```bash
   conan install . --output-folder=build --build=missing
   ```

2. Configure the build with CMake:
   ```bash
   cd build
   cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
   ```

3. Build the applications:
   ```bash
   cmake --build .
   ```

#### Option 2: One-command build

From the project root directory:
```bash
conan install . --output-folder=build --build=missing && \
cd build && \
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release && \
cmake --build .
```

#### Build Types

For a debug build, use `-DCMAKE_BUILD_TYPE=Debug` instead of `Release`:
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
```

Note: You'll need to run `conan install` again with `--settings build_type=Debug` for debug builds.

#### Clean Build

To start fresh:
```bash
rm -rf build
```

### Running AHLMO

AHLMO supports a small suite of unit tests; run them by invoking:

```bash
./build/acme-unit-tests
```

Run the AHLMO simulator with this command:

```bash
./build/acme-mining N M
```

where `N` is the number of trucks, and `M` is the number of unloading stations.

AHLMO will take about 3-1/2 minutes to simulate a 72-hour mining day, and will produce a log and several time-stamped `CSV` files suitable for further statistical analysis.

### Notes

- Conan manages all external dependencies (currently Google Test)
- The `--build=missing` flag ensures Conan builds any dependencies not available as pre-built binaries
- No system-wide installation of Google Test is required

---

## Problem Statement

### Objective

This simulation was developed to manage a lunar Helium-3 space mining operation, tracking the efficiency of mining trucks and unload stations over a continuous 72-hour operation.

### Key Components

**Mining Trucks:** Autonomous vehicles that perform the actual mining tasks.

**Mining Sites:** Locations on the moon where trucks extract Helium-3. The simulation assumes an infinite number of sites, ensuring trucks always have access to mine without waiting.

**Mining Unload Stations:** Designated stations where trucks unload the mined Helium-3. Each station can handle one truck at a time.

### Operation Details

- There are *n* mining trucks and *m* mining unload stations (configurable)
- Mining trucks spend a random duration between 1 to 5 hours mining at the sites
- Travel time between a mining site and an unload station is 30 minutes
- All trucks start empty at a mining site when the simulation begins
- Unloading mined Helium-3 at a station takes 5 minutes
- Trucks are assigned to the first available unload station
- If all stations are occupied, trucks queue at the station with the shortest wait time and remain in their chosen queue

### Simulation Requirements

- Configurable to accommodate various numbers of mining trucks (*n*) and unload stations (*m*)
- Calculates and reports statistics for the performance and efficiency of each mining truck and unload station
- Represents 72 hours of non-stop mining
- Executes faster than real-time to provide timely analysis

### Implementation Approach

This project is implemented in C++ leveraging object-oriented programming principles where appropriate, including:
- State machine-based truck control
- Event-driven simulation architecture
- Modular design with clear separation of concerns
- Comprehensive logging and statistical analysis output
