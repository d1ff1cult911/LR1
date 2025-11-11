#include "../include/Lazy_sequence.h"
#include <cassert>

void run_tests() {
    // Fibonacci
    {
        std::vector<long long> seeds = {1,1};
        auto fibGen = [](LazySequence<long long>* self, size_t i) -> long long {
            if (i < 2) return self->Get(i);
            return self->Get(i-1) + self->Get(i-2);
        };
        LazySequence<long long> fib(seeds, fibGen);
        std::vector<long long> expect = {1,1,2,3,5,8,13,21,34,55};
        for (size_t i=0;i<expect.size();++i)
            assert(fib.Get(i) == expect[i]);
    }

    // Factorials
    {
        std::vector<long long> seed = {1};
        auto factGen = [](LazySequence<long long>* self, size_t i) -> long long {
            if (i==0) return 1;
            return self->Get(i-1) * (long long)i;
        };
        LazySequence<long long> fact(seed, factGen);
        assert(fact.Get(5) == 120);
    }

    // Map & Where
    {
        std::vector<int> base = {1,2,3,4,5,6};
        auto baseSeq = std::make_shared<LazySequence<int>>(base);
        auto sq = baseSeq->Map<int>([](const int& x){ return x*x; });
        assert(sq.Get(3) == 16);

        auto even = baseSeq->Where([](const int& x){ return x%2==0; });
        assert(even.Get(2) == 6);
    }

    // Stream
    {
        std::vector<int> base = {10,20,30};
        auto seq = std::make_shared<LazySequence<int>>(base);
        ReadOnlyStream<int> s(seq);
        assert(s.Read() == 10);
        assert(s.Read() == 20);
        assert(s.Read() == 30);
        assert(s.IsEndOfStream());
    }

    std::cout << "✅ All tests passed successfully.\n";
}
