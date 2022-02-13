#include <array>
#include <iostream>
#include <string_view>

#include "../better_split.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#ifdef AOC_BENCH
#include <chrono>
using std::chrono::microseconds;
using std::chrono::duration_cast;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::atoll;
using std::cout;
using std::string_view;

using Int = long long;

template <Int N>
Int solution(const string_view str) {
    array<Int, 9> map{};
    for (const Int n : str | split(',') | transform([](const string_view pat) {
                           return atoll(pat.data());
                       }))
        map[n]++;
    for (const Int n : iota(1ll, N)) map[(n + 7) % 9] += map[n % 9];
    return accumulate(map, 0ull);
}

int main() {
    const string_view str{
#include "../../input/input06.txt"
    };
#ifdef AOC_BENCH
    const auto point1{system_clock::now()};
    const Int res1{solution<80ll>(str)};
    const auto point2{system_clock::now()};
    const Int res2{solution<256ll>(str)};
    const auto point3{system_clock::now()};
    cout << res1 << '\n'
         << res2 << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 1000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 1000.
         << "ms\n";
#else
    cout << solution<80ll>(str) << '\n' << solution<256ll>(str) << '\n';
#endif
    return 0;
}
