#include "LazyMultiTuringMachine.h"

LazyMultiTuringMachine::LazyMultiTuringMachine(int K_, char blank)
    : K(K_), state("")
{
    tapes.reserve(K);
    heads.reserve(K);
    for (int i = 0; i < K; i++) {
        tapes.emplace_back(blank);
        heads.push_back(0);
    }
}

void LazyMultiTuringMachine::setStartState(const std::string& s) {
    state = s;
}

void LazyMultiTuringMachine::addAcceptState(const std::string& s) {
    acceptStates[s] = true;
}

void LazyMultiTuringMachine::addTransition(
    const std::string& fromState,
    const std::vector<char>& read,
    const std::vector<char>& write,
    const std::vector<int>& move,
    const std::string& toState
) {
    transitions[{fromState, read}] =
        MultiTMTransition{write, move, toState};
}

void LazyMultiTuringMachine::loadTape(int tapeIndex, const std::string& data, long long offset) {
    for (size_t i = 0; i < data.size(); i++)
        tapes[tapeIndex].write(offset + i, data[i]);
}

void LazyMultiTuringMachine::setHeadPosition(int tapeIndex, long long pos) {
    heads[tapeIndex] = pos;
}

bool LazyMultiTuringMachine::step() {
    if (acceptStates.count(state)) return false;

    std::vector<char> read(K);
    for (int i = 0; i < K; i++)
        read[i] = tapes[i].read(heads[i]);

    auto it = transitions.find({state, read});
    if (it == transitions.end()) return false;

    MultiTMTransition t = it->second;

    for (int i = 0; i < K; i++)
        tapes[i].write(heads[i], t.write[i]);

    for (int i = 0; i < K; i++)
        heads[i] += t.move[i];

    state = t.nextState;

    return !acceptStates.count(state);
}

bool LazyMultiTuringMachine::run(long long maxSteps) {
    for (long long i = 0; i < maxSteps; i++) {
        bool cont = step();
        if (!cont) return acceptStates.count(state);
    }
    return false;
}

void LazyMultiTuringMachine::dumpState(long long span) const {
    std::cout << "State: " << state << "\n";
    for (int i = 0; i < K; i++) {
        std::cout << "Tape " << i << ": "
                  << tapes[i].snapshot(heads[i], span) << "\n";
    }
}
