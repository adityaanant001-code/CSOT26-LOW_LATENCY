#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<sstream>
#include<deque>
#include<unordered_map>

#include "../include/strategy.hpp"

using namespace std;


vector<csot::Tick>load_ticks(const string& path){
    vector<csot::Tick>ticks;

    deque<string> symbol_storage;
    unordered_map<string, string_view> interned;

    ifstream file(path);

    if(!file){
        return ticks;
    }

    string line;
    getline(file,line);

    while(getline(file,line)){
        stringstream ss(line);

        string timestamp;
        string symbol;
        string bid_px;
        string ask_px;
        string bid_qty;
        string ask_qty;

        getline(ss, timestamp, ',');
        getline(ss, symbol, ',');
        getline(ss, bid_px, ',');
        getline(ss, ask_px, ',');
        getline(ss, bid_qty, ',');
        getline(ss, ask_qty, ',');

        auto it = interned.find(symbol);

        if(it == interned.end())
        {
            symbol_storage.push_back(symbol);

            string_view sv = symbol_storage.back();

            interned[symbol] = sv;

            it = interned.find(symbol);
        }

        csot::Tick t;

        t.timestamp_ns = stoull(timestamp);

        t.bid_px = stod(bid_px);
        t.ask_px = stod(ask_px);

        t.bid_qty = stoul(bid_qty);
        t.ask_qty = stoul(ask_qty);

        t.symbol=it->second;

        ticks.push_back(t);

    }

    return ticks;


}

