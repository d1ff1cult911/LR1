#pragma once
#include "Lazy_sequence.h"
#include <string>

class LazyTape {
public:
    explicit LazyTape(char blank_ = '_') : blank(blank_) {
        auto gen = [this](LazySequence<char>*, size_t)->char {
            return blank;
        };
        pos = std::make_shared<LazySequence<char>>(std::vector<char>{}, gen);
        neg = std::make_shared<LazySequence<char>>(std::vector<char>{}, gen);
    }

    char read(long long idx) const {
        if (idx >= 0) return pos->Get(idx);
        return neg->Get(-idx - 1);
    }

    void write(long long idx, char v) {
        if (idx >= 0) pos->directSet(idx, v);
        else neg->directSet(-idx - 1, v);
    }

    std::string snapshot(long long head, long long span = 20) const {
        std::string out;
        long long L = head - span;
        long long R = head + span;
        for (long long p = L; p <= R; p++) {
            char c = read(p);
            if (p == head) out += "[" + std::string(1,c) + "]";
            else out += " " + std::string(1,c) + " ";
        }
        return out;
    }

private:
    char blank;
    std::shared_ptr<LazySequence<char>> pos;
    std::shared_ptr<LazySequence<char>> neg;
};
