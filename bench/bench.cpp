#include <benchmark/benchmark.h>


#include "../include/spec_strategy.hpp"
#include "../include/strategy.hpp"
using namespace std;



vector<csot::Tick>load_ticks(const string& path);

static void BM_OnTick(benchmark::State& state){

    auto ticks=load_ticks("data/synthetic_large.csv");
    

    for(auto _:state){
        SpecStrategy strategy;
        
        for(const auto& tick :ticks){
            benchmark::DoNotOptimize(strategy.on_tick(tick));
        }

    }

    state.SetItemsProcessed(state.iterations()*ticks.size());
}

BENCHMARK(BM_OnTick);
BENCHMARK_MAIN();