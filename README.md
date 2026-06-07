# CSoT'26 Low Latency Strategy

## Overview

This project implements the Week 1 reference strategy for the CSoT'26 Low Latency cohort challenge.

The strategy maintains a rolling 64-tick window per symbol and performs mean-reversion trading based on deviations from the rolling mean.

## Strategy

* Rolling window size: 64 ticks
* O(1) rolling mean update
* O(1) rolling variance update
* Array-based symbol state storage
* Mean-reversion entry and exit logic
* Supports up to 64 symbols

## Performance

Judge results:

* p50 latency: 27 ns
* p99 latency: 76 ns
* p999 latency: 110 ns
* Throughput: 16.43 M ticks/s

## Build

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## Output

The strategy shared object is generated as:

```text
build/spec_strategy.so
```

## Hardware Notes

Development machine:

ASUS Vivobook S14
intel Core Ultra processor
Model name:13th Gen Intel(R) Core(TM) i7-13620H
kernel: 7.0.0-22-generic
RAM: 16GB
OS: Ubuntu Linux

## Repository Contents

* `src/` – engine and loader
* `strategies/` – strategy implementation
* `include/` – headers
* `bench/` – benchmark code
* `data/gen.py` – synthetic data generator

## Surprising Observation

The most surprising observation was that replacing `std::unordered_map` with a fixed-size `std::array` indexed directly by symbol ID produced a larger performance improvement than many arithmetic-level optimizations. Even though the strategy logic remained unchanged, removing hashing and pointer chasing significantly reduced latency and improved throughput. This highlighted how memory access patterns and cache friendliness can matter more than reducing a few floating-point operations in low-latency systems.


Large generated datasets are intentionally excluded from version control.

