#pragma once
#include "LazyTape.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>

struct MultiTMTransition {
    std::vector<char> write;
    std::vector<int> move;
    std::string nextState;
};

class LazyMultiTuringMachine {
public:
    LazyMultiTuringMachine(int K, char blank = '_');

    void setStartState(const std::string& s);
    void addAcceptState(const std::string& s);

    void addTransition(
        const std::string& fromState,
        const std::vector<char>& read,
        const std::vector<char>& write,
        const std::vector<int>& move,
        const std::string& toState
    );

    void loadTape(int tapeIndex, const std::string& data, long long offset = 0);
    void setHeadPosition(int tapeIndex, long long pos);

    bool step();
    bool run(long long maxSteps = 1000000);

    void dumpState(long long span = 20) const;

private:
    int K;
    std::vector<LazyTape> tapes;
    std::vector<long long> heads;

    std::string state;
    std::unordered_map<std::string,bool> acceptStates;

    using Key = std::pair<std::string, std::vector<char>>;
    struct KeyCompare {
        bool operator()(Key const& a, Key const& b) const {
            if (a.first != b.first) return a.first < b.first;
            return a.second < b.second;
        }
    };

    std::map<Key, MultiTMTransition, KeyCompare> transitions;
};
