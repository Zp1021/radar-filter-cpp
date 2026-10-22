# radar-filter-cpp

A C++17 alpha-beta tracking filter, written as a command-line tool. It is a C++ version of the
alpha-beta filter idea from my JavaScript
[RadarTracker](https://github.com/Zp1021/RadarTracker), with its own gains (alpha 0.5, beta 0.1)
and tests.

## What it does
Reads noisy 2D position measurements ('t,x,y') from a CSV file, runs one alpha-beta filter on x
and one on y, and prints the smoothed position and estimated velocity at each step.

Each update has three parts:
1. **Predict** where the target should be, from the last position and velocity.
2. **Measure the residual**: how far the new reading is from the prediction.
3. **Correct** position (by alpha) and velocity (by beta) toward the reading.

## Build and run
```bash
g++ -std=c++17 -Wall -Wextra alpha_beta.cpp main.cpp -o tracker
./tracker measurements.csv
```
## Test
```bash
g++ -std=c++17 -Wall -Wextra alpha_beta.cpp test_alpha_beta.cpp -o test_alpha_beta
./test_alpha_beta
```

Tests check a stationary target, learning a constant velocity, damping a noise spike, and rejecting invalid settings. They run on every push with GitHub Actions.

## Design notes
- Filter logic lives in its own class (`alpha_beta.hpp/.cpp`), separate from file input/output, so it can be tested on its own.
- Invalid settings (alpha outside (0,1], beta outside (0,2], dt not positive) throw `std::invalid_argument`.
- No external libraries; standard C++17 only.

## What I'd add next
A full Kalman filter (adaptive gains from noise covariances), CMake, and GoogleTest.