#pragma once
#include <functional>
#include <vector>
#include <optional>
#include <memory>
#include <stdexcept>
#include <iostream>

template<typename T>
class LazySequence {
public:
    using GenFunc = std::function<T(LazySequence<T>*, size_t)>;

    LazySequence();
    LazySequence(const std::vector<T>& items);
    LazySequence(const std::vector<T>& firstItems, GenFunc generator, std::optional<size_t> length = std::nullopt);

    T Get(size_t index);
    T GetFirst();
    size_t GetMaterializedCount() const;
    size_t GetLengthIfKnown() const;

    void Append(const T& item);

    template<typename U>
    LazySequence<U> Map(std::function<U(const T&)> f);

    LazySequence<T> Where(std::function<bool(const T&)> pred);
    LazySequence<T> Concat(std::shared_ptr<LazySequence<T>> other);

private:
    std::vector<T> memo;
    GenFunc gen;
    std::optional<size_t> known_length;
};

// ---------- ReadOnlyStream ----------
template<typename T>
class ReadOnlyStream {
public:
    ReadOnlyStream(std::shared_ptr<LazySequence<T>> src);
    bool IsEndOfStream();
    T Read();
    size_t GetPosition() const;
    void Seek(size_t index);

private:
    std::shared_ptr<LazySequence<T>> source;
    size_t pos;
};

#include "../src/Lazy_sequence.cpp"
