#pragma once
#include <functional>
#include <vector>
#include <optional>
#include <memory>
#include <stdexcept>

template<typename T>
class LazySequence {
public:
    using GenFunc = std::function<T(LazySequence<T>*, size_t)>;

    LazySequence() : gen(nullptr), known_length(std::nullopt) {}

    LazySequence(const std::vector<T>& items)
        : memo(items), gen(nullptr), known_length(items.size()) {}

    LazySequence(const std::vector<T>& firstItems, GenFunc generator,
                 std::optional<size_t> length = std::nullopt)
        : memo(firstItems), gen(generator), known_length(length) {}

    T Get(size_t index) {
        if (known_length && index >= *known_length)
            throw std::out_of_range("Index >= known length");

        while (memo.size() <= index) {
            if (!gen) throw std::out_of_range("Index out of range");
            size_t i = memo.size();
            memo.push_back(gen(this, i));
        }
        return memo[index];
    }

    void directSet(size_t index, const T& value) {
        if (memo.size() <= index)
            Get(index);
        memo[index] = value;
    }

    template<typename U>
    LazySequence<U> Map(std::function<U(const T&)> f) {
        auto generator = [this, f](LazySequence<U>* owner, size_t i)->U {
            return f(this->Get(i));
        };
        std::vector<U> empty;
        return LazySequence<U>(empty, generator, known_length);
    }

    LazySequence<T> Where(std::function<bool(const T&)> pred) {
        struct State {
            LazySequence<T>* src;
            std::function<bool(const T&)> pred;
            size_t last = 0;
            std::vector<T> out;
        };
        auto st = std::make_shared<State>();
        st->src = this;
        st->pred = pred;

        auto generator = [st](LazySequence<T>* owner, size_t i)->T {
            while (st->out.size() <= i) {
                T v = st->src->Get(st->last++);
                if (st->pred(v))
                    st->out.push_back(v);
            }
            return st->out[i];
        };

        std::vector<T> empty;
        return LazySequence<T>(empty, generator);
    }

private:
    std::vector<T> memo;
    GenFunc gen;
    std::optional<size_t> known_length;
};


// ------------------------- ReadOnlyStream -------------------------
template<typename T>
class ReadOnlyStream {
public:
    ReadOnlyStream(std::shared_ptr<LazySequence<T>> src)
        : source(src), pos(0) {}

    bool IsEndOfStream() {
        try { source->Get(pos); return false; }
        catch (...) { return true; }
    }

    T Read() {
        if (IsEndOfStream()) throw std::runtime_error("End");
        return source->Get(pos++);
    }

private:
    std::shared_ptr<LazySequence<T>> source;
    size_t pos;
};
