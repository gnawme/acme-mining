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
