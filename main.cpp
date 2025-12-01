#include <iostream>
#include "include/LazyMultiTuringMachine.h"

void demo_two_tape_copy() {
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

    std::string s;
    std::cout << "Input unary string: ";
    std::cin >> s;

    tm.loadTape(0, s);
    tm.setHeadPosition(0,0);
    tm.setHeadPosition(1,0);

    std::cout << "Before:\n";
    tm.dumpState(10);

    bool ok = tm.run(10000);

    std::cout << "After:\n";
    tm.dumpState(10);

    std::cout << "Accepted? " << (ok?"yes":"no") << "\n";
}

int main() {
    while (true) {
        std::cout << "1) Demo 2-tape copy machine\n0) Exit\n";
        int c;
        std::cin >> c;
        if (c==0) break;
        if (c==1) demo_two_tape_copy();
        else std::cout << "Unknown\n";
    }
}
