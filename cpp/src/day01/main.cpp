#include <iostream>
#include <string_view>

#include "../better_split.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/iterator/operations.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::count_if;
using ranges::next;
using ranges::views::sliding;
using ranges::views::split;
using ranges::views::transform;
using std::atoll;
using std::cout;
using std::string_view;

using Int = long long;

template <Int N>
Int solution(const string_view str) {
    return count_if(str | split('\n') | transform([](const string_view pat) {
                        return atoll(pat.data());
                    }) | sliding(N),
                    [](const auto &unk) {
                        return *unk.begin() < *next(unk.begin(), N - 1);
                    });
}

int main() {
    const string_view str{
#include "../../input/input01.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<2ll>(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<4ll>(str);
    const auto point3{system_clock::now()};
    cout << '\n'
         << "Day01"
         << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 100000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 100000.
         << "ms\n";
#else
    cout << solution<2ll>(str) << '\n' << solution<4ll>(str) << '\n';
#endif
    return 0;
}
