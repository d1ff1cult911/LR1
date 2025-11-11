#pragma once
#include "../include/Lazy_sequence.h"

// ---------- LazySequence Implementation ----------

template<typename T>
LazySequence<T>::LazySequence() : gen(nullptr), known_length(std::nullopt) {}

template<typename T>
LazySequence<T>::LazySequence(const std::vector<T>& items)
    : memo(items), gen(nullptr), known_length(items.size()) {}

template<typename T>
LazySequence<T>::LazySequence(const std::vector<T>& firstItems, GenFunc generator, std::optional<size_t> length)
    : memo(firstItems), gen(generator), known_length(length) {}

template<typename T>
T LazySequence<T>::Get(size_t index) {
    if (known_length && index >= *known_length)
        throw std::out_of_range("Index >= known length");
    while (memo.size() <= index) {
        if (!gen) throw std::out_of_range("Index out of range and no generator");
        size_t i = memo.size();
        T val = gen(this, i);
        memo.push_back(val);
    }
    return memo[index];
}

template<typename T>
T LazySequence<T>::GetFirst() {
    if (memo.empty()) return Get(0);
    return memo[0];
}

template<typename T>
size_t LazySequence<T>::GetMaterializedCount() const { return memo.size(); }

template<typename T>
size_t LazySequence<T>::GetLengthIfKnown() const {
    return known_length ? *known_length : SIZE_MAX;
}

template<typename T>
void LazySequence<T>::Append(const T& item) {
    memo.push_back(item);
    if (known_length) ++(*known_length);
}

template<typename T>
template<typename U>
LazySequence<U> LazySequence<T>::Map(std::function<U(const T&)> f) {
    std::vector<U> empty;
    auto generator = [this, f](LazySequence<U>* owner, size_t idx) -> U {
        T src = this->Get(idx);
        return f(src);
    };
    return LazySequence<U>(empty, generator, this->known_length);
}

template<typename T>
LazySequence<T> LazySequence<T>::Where(std::function<bool(const T&)> pred) {
    struct State {
        LazySequence<T>* src;
        std::function<bool(const T&)> pred;
        size_t lastChecked = 0;
        std::vector<T> out_memo;
    };
    auto state = std::make_shared<State>();
    state->src = this;
    state->pred = pred;

    auto generator = [state](LazySequence<T>* owner, size_t idx) -> T {
        while (state->out_memo.size() <= idx) {
            size_t j = state->lastChecked;
            T cand;
            while (true) {
                try {
                    cand = state->src->Get(j++);
                } catch (const std::out_of_range&) {
                    throw std::out_of_range("No more elements matching predicate");
                }
                if (state->pred(cand)) {
                    state->out_memo.push_back(cand);
                    break;
                }
            }
            state->lastChecked = j;
        }
        return state->out_memo[idx];
    };

    std::vector<T> empty;
    return LazySequence<T>(empty, generator);
}

template<typename T>
LazySequence<T> LazySequence<T>::Concat(std::shared_ptr<LazySequence<T>> other) {
    std::vector<T> empty;
    auto generator = [this, other](LazySequence<T>* owner, size_t idx) -> T {
        try {
            return this->Get(idx);
        } catch (const std::out_of_range&) {
            if (!this->known_length) throw std::out_of_range("Cannot reach concatenated part (infinite)");
            size_t offset = *this->known_length;
            return other->Get(idx - offset);
        }
    };
    std::optional<size_t> total_len = std::nullopt;
    if (this->known_length && other->known_length)
        total_len = *this->known_length + *other->known_length;
    return LazySequence<T>(empty, generator, total_len);
}

// ---------- ReadOnlyStream Implementation ----------

template<typename T>
ReadOnlyStream<T>::ReadOnlyStream(std::shared_ptr<LazySequence<T>> src)
    : source(src), pos(0) {}

template<typename T>
bool ReadOnlyStream<T>::IsEndOfStream() {
    if (source->GetLengthIfKnown() != SIZE_MAX)
        return pos >= source->GetLengthIfKnown();
    try { source->Get(pos); return false; } catch (...) { return true; }
}

template<typename T>
T ReadOnlyStream<T>::Read() {
    if (IsEndOfStream()) throw std::runtime_error("EndOfStream");
    T val = source->Get(pos);
    ++pos;
    return val;
}

template<typename T>
size_t ReadOnlyStream<T>::GetPosition() const { return pos; }

template<typename T>
void ReadOnlyStream<T>::Seek(size_t index) {
    source->Get(index);
    pos = index;
}
