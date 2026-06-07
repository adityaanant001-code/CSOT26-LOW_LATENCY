#pragma once

#include "strategy.hpp"

class NullStrategy : public csot::Strategy
{
public:
    std::vector<csot::Order> on_tick(const csot::Tick&) override;
};