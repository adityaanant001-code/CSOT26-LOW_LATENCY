#include "../include/spec_strategy.hpp"

#include <cmath>

using namespace std;

std::vector<csot::Order>
SpecStrategy::on_tick(const csot::Tick& t){

    auto& st = states[symbol_index(t.symbol)];

    const double mid =
        (t.bid_px + t.ask_px) * 0.5;

    const double old =
        st.mids[st.head];

    st.mids[st.head] = mid;

    st.head =
        (st.head + 1) & 63;

    if(st.count < 64){
    
        ++st.count;

        st.sum += mid;

        st.sum_sq += mid * mid;
    
    }else{
    
        st.sum += mid - old;

        st.sum_sq +=
            mid * mid -
            old * old;
    }

    if(st.count < 64){
    
        return {};
    }

    const double mean =
    st.sum / 64.0;

    const double variance =
    st.sum_sq / 64.0 -
    mean * mean;

    if(variance <= 1e-18){
    
        return {};
    }

    const double diff =mid-mean;

    const double diff_sq =diff * diff;
    

    if(st.position == 0){

        if(diff > 0.0 && diff_sq >= 4.0 * variance){
    
            return {
                csot::Order{
                    csot::Order::Side::SELL,
                    t.symbol,
                    t.bid_px,
                    1
                }
            };
        }

        if(diff < 0.0 && diff_sq >= 4.0 * variance){
    
            return {
                csot::Order{
                    csot::Order::Side::BUY,
                    t.symbol,
                    t.ask_px,
                    1
                }
            };
        }

        return {};
    }

    if(st.position > 0 && diff_sq <= 0.25 * variance){
        return {
            csot::Order{
                csot::Order::Side::SELL,
                t.symbol,
                t.bid_px,
                static_cast<uint32_t>(
                    st.position
                )
            }
        };
    }

    if(st.position < 0 && diff_sq <= 0.25 * variance){
        return {
            csot::Order{
                csot::Order::Side::BUY,
                t.symbol,
                t.ask_px,
                static_cast<uint32_t>(
                    -st.position
                )
            }
        };
    }

    return{};
}





void SpecStrategy::on_fill(
    const csot::Order& o,
    double,
    uint32_t fill_qty)
{
    auto& st =
        states[symbol_index(o.symbol)];

    if(o.side ==
       csot::Order::Side::BUY)
    {
        st.position +=
            static_cast<int32_t>(
                fill_qty
            );
    }
    else
    {
        st.position -=
            static_cast<int32_t>(
                fill_qty
            );
    }
}

extern "C"
csot::Strategy* create_strategy()
{
    return new SpecStrategy();
}