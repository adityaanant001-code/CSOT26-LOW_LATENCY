#include <iostream>
#include <vector>

#include "../include/strategy.hpp"
#include "../include/engine.hpp"
#include "../include/spec_strategy.hpp"

using namespace std;

vector<csot::Tick> load_ticks(const string& path);

int main()
{
    auto ticks = load_ticks("data/synthetic_large.csv");
    cin.get();

    SpecStrategy strategy;

    Engine engine;

    engine.run(ticks, strategy);

    return 0;
}