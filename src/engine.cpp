#include "../include/engine.hpp"

#include <chrono>
#include <iostream>
#include <x86intrin.h>

using namespace std;

namespace
{
    double calibrate_cycles_to_ns()
    {
        constexpr int N = 1000000;

        auto wall_start = chrono::steady_clock::now();

        _mm_lfence();
        uint64_t tsc_start = __rdtsc();
        _mm_lfence();

        for(int i = 0; i < N; ++i)
        {
            asm volatile("" ::: "memory");
        }

        _mm_lfence();
        uint64_t tsc_end = __rdtsc();
        _mm_lfence();

        auto wall_end = chrono::steady_clock::now();

        auto ns =
            chrono::duration_cast<chrono::nanoseconds>(
                wall_end - wall_start
            ).count();

        uint64_t cycles = tsc_end - tsc_start;

        return static_cast<double>(ns) /
               static_cast<double>(cycles);
    }
}

void Engine::run(
    const vector<csot::Tick>& ticks,
    csot::Strategy& strategy)
{
    csot::LatencyHistogram hist;

    strategy.on_init();

    const double ns_per_cycle =
        calibrate_cycles_to_ns();

    for(const auto& tick : ticks)
    {
        _mm_lfence();
        uint64_t start = __rdtsc();
        _mm_lfence();

        auto orders = strategy.on_tick(tick);

        _mm_lfence();
        uint64_t end = __rdtsc();
        _mm_lfence();

        uint64_t cycles = end - start;

        uint64_t ns =
            static_cast<uint64_t>(
                cycles * ns_per_cycle
            );

        hist.record(ns);

        for(const auto& order : orders)
        {
            strategy.on_fill(
                order,
                order.price,
                order.qty
            );
        }
    }

    cout << ticks.size()
         << "ticks processed"
         << '\n';

    hist.print(cout);
}