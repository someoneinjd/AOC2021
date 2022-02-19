#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <utility>

#include "../better_split.hpp"
#include "range/v3/view/generate.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/take.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/zip.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::views::generate;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::take;
using ranges::views::transform;
using ranges::views::zip;
using std::abs;
using std::array;
using std::cout;
using std::exchange;
using std::isdigit;
using std::max;
using std::min;
using std::string_view;
using std::strtoll;

using Int = long long;

array<char, 1000000> map{};

array<Int, 4> parse(const string_view str) {
    // No regex !!!
    array<Int, 4> res{};
    const char *begin{str.data()};
    char *end{nullptr};
    for (auto &i : res) {
        while (!isdigit(*begin)) begin++;
        i = strtoll(begin, &end, 10);
        begin = end;
    }
    return res;
}

Int solution1(const string_view str) {
    Int overlaps{0ll};
    for (const auto [x, y, xx, yy] :
         str | split('\n') | transform([](const string_view pat) {
             const auto [x, y, xx, yy]{parse(pat)};
             return array{min(x, xx), min(y, yy), max(x, xx), max(y, yy)};
         })) {
        if (x == xx)
            for (const Int y : iota(y, yy + 1))
                overlaps += map[x + y * 1000]++ == 1;
        else if (y == yy)
            for (const Int x : iota(x, xx + 1))
                overlaps += map[x + y * 1000]++ == 1;
    }
    return overlaps;
}

Int solution2(const string_view str) {
    map.fill(0ll);
    Int overlaps{0ll};
    for (const string_view pat : str | split('\n')) {
        const auto [x, y, xx, yy] = parse(pat);
        Int prev1{x};
        Int prev2{y};
        for (const auto [x, y] :
             zip(
                 [&] {
                     return generate([&prev1, x, xx] {
                         return exchange(
                             prev1,
                             prev1 + (xx == x ? 0ll : (xx > x ? 1ll : -1ll)));
                     });
                 }(),
                 [&] {
                     return generate([&prev2, y, yy] {
                         return exchange(
                             prev2,
                             prev2 + (yy == y ? 0ll : (yy > y ? 1ll : -1ll)));
                     });
                 }()) |
                 take(max(abs(xx - x), abs(yy - y)) + 1)) {
            overlaps += map[x + y * 1000]++ == 1;
        }
    }
    return overlaps;
}

int main() {
    const string_view str{
#include "../../input/input05.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay05"
         << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 100000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 100000.
         << "ms\n";
#else
    cout << solution1(str) << '\n' << solution2(str) << '\n';
#endif
    return 0;
}
