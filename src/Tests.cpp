#include <iostream>
#include "Lazy_sequence.h"
#include "LazyMultiTuringMachine.h"

void test_lazy_sequence() {
    std::cout << "Testing LazySequence...\n";

    std::vector<long long> start = {1,1};
    auto fibGen = [](LazySequence<long long>* self, size_t i)->long long {
        if (i < 2) return self->Get(i);
        return self->Get(i-1) + self->Get(i-2);
    };

    LazySequence<long long> fib(start, fibGen);

    std::cout << "First 10 Fibonacci numbers: ";
    for (int i=0;i<10;i++)
        std::cout << fib.Get(i) << " ";
    std::cout << "\n";
}

void test_multitape_copy() {
    std::cout << "Testing 2-tape copy machine...\n";
    LazyMultiTuringMachine tm(2, '_');
    tm.setStartState("q0");
    tm.addAcceptState("q_accept");

    tm.addTransition(
        "q0",
        {'1','_'},
        {'1','1'},
        {+1,+1},
        "q0"
    );

    tm.addTransition(
        "q0",
        {'_','_'},
        {'_','_'},
        {0,0},
        "q_accept"
    );

    tm.loadTape(0, "111", 0);
    tm.setHeadPosition(0,0);
    tm.setHeadPosition(1,0);

    bool ok = tm.run(1000);
    std::cout << "Copy result:\n";
    tm.dumpState(10);
}

int main() {
    
    test_lazy_sequence();
    test_multitape_copy();
    return 0;
}
