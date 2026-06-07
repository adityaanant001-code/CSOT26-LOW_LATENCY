#pragma once
#include<cassert>

#include "strategy.hpp"

#include <array>
#include <string_view>
#include <vector>

struct SymbolState
{
    double mids[64]{};

    uint32_t count = 0;

    uint32_t head = 0;

    int32_t position = 0;

    double sum = 0.0;

    double sum_sq = 0.0;
};

class SpecStrategy : public csot::Strategy
{
private:

    std::array<SymbolState, 64> states;

    static inline int symbol_index(std::string_view sym)
    {
    int idx = 0;

    for(size_t i = 3; i < sym.size(); ++i)
    {
        idx = idx * 10 + (sym[i] - '0');
    }

    assert(idx >= 0 && idx < 64);

    return idx;
    }
public:

    std::vector<csot::Order>
    on_tick(const csot::Tick& t) override;

    void on_fill(
        const csot::Order& o,
        double fill_price,
        uint32_t fill_qty
    ) override;
};