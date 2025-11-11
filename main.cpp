#include "include/Lazy_sequence.h"
#include <iostream>

void run_tests();

void demo_menu() {
    std::cout << "=== ЛР1: Ленивые последовательности ===\n";
    std::cout << "1) Показать N чисел Фибоначчи\n";
    std::cout << "2) Показать N факториалов\n";
    std::cout << "3) Демонстрация Map/Where\n";
    std::cout << "4) Прогнать тесты\n";
    std::cout << "0) Выход\n";

    int choice;
    while (true) {
        std::cout << "\nВаш выбор: ";
        if (!(std::cin >> choice)) break;
        if (choice == 0) break;

        if (choice == 1) {
            int N; std::cout << "N = "; std::cin >> N;
            std::vector<long long> seeds = {1,1};
            auto fibGen = [](LazySequence<long long>* self, size_t i)->long long {
                if (i<2) return self->Get(i);
                return self->Get(i-1)+self->Get(i-2);
            };
            LazySequence<long long> fib(seeds, fibGen);
            for (int i=0;i<N;++i)
                std::cout << fib.Get(i) << ' ';
            std::cout << '\n';
        }

        else if (choice == 2) {
            int N; std::cout << "N = "; std::cin >> N;
            std::vector<long long> seed = {1};
            auto factGen = [](LazySequence<long long>* self, size_t i)->long long {
                if (i==0) return 1;
                return self->Get(i-1)*(long long)i;
            };
            LazySequence<long long> fact(seed, factGen);
            for (int i=0;i<N;++i)
                std::cout << fact.Get(i) << ' ';
            std::cout << '\n';
        }

        else if (choice == 3) {
            std::vector<int> base = {1,2,3,4,5,6,7,8};
            auto seq = std::make_shared<LazySequence<int>>(base);
            auto sq = seq->Map<int>([](const int& x){ return x*x; });
            auto even = seq->Where([](const int& x){ return x%2==0; });

            std::cout << "Squares: ";
            for (int i=0;i<5;++i) std::cout << sq.Get(i) << ' ';
            std::cout << "\nEvens: ";
            for (int i=0;i<4;++i) std::cout << even.Get(i) << ' ';
            std::cout << '\n';
        }

        else if (choice == 4) {
            run_tests();
        }

        else std::cout << "Неизвестный пункт меню\n";
    }
}

int main() {
    demo_menu();
    return 0;
}
