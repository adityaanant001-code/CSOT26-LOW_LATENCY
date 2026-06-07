#pragma once

#include<string>
#include<vector>

#include"strategy.hpp"
#include"histogram.hpp"
#include<iostream>

using namespace std;

class Engine{
public:
    void run(const vector<csot::Tick>& ticks,csot::Strategy& strategy);
};