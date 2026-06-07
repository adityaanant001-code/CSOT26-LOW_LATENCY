#include<iostream>
#include<vector>
#include "../include/strategy.hpp"

using namespace std;

#include "../include/null_strategy.hpp"

std::vector<csot::Order>
NullStrategy::on_tick(const csot::Tick&)
{
    return {};
}

extern "C"
csot::Strategy* create_strategy()
{
    return new NullStrategy();
}