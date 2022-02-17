#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::find_if;
using ranges::to;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::cout;
using std::pair;
using std::string;
using std::string_view;
using std::vector;

using Int = long long;

const array<pair<Int, Int>, 8> neighbours{
    pair{-1ll, -1ll}, pair{0ll, -1ll}, pair{1ll, -1ll}, pair{1ll, 0ll},
    pair{-1ll, 0ll},  pair{-1ll, 1ll}, pair{0ll, 1ll},  pair{1ll, 1ll}};

Int solution_helper(vector<string> &map, Int x, Int y) {
    map[y][x] = '0';
    return accumulate(
        neighbours | transform([=](const pair<Int, Int> &pat) {
            const auto [dx, dy] = pat;
            return pair{dx + x, dy + y};
        }),
        1ll, [&](const Int acc, const pair<Int, Int> &pat) {
            const auto [x, y] = pat;
            return acc +
                   (x < 10 && y < 10 && x > -1 && y > -1 && map[y][x] > '0'
                        ? (++map[y][x] > '9' ? solution_helper(map, x, y) : 0ll)
                        : 0ll);
        });
}

Int solution1(const string_view str) {
    auto map{str | split('\n') | to<vector<string>>()};
    Int res{0ll};
    for (const auto _ : iota(0, 100)) {
        for (auto &row : map)
            for (auto &cell : row) cell++;
        for (const Int x : iota(0ll, 10ll))
            for (const Int y : iota(0ll, 10ll))
                res += map[y][x] > '9' ? solution_helper(map, x, y) : 0ll;
    }
    return res;
}

Int solution2(const string_view str) {
    auto map{str | split('\n') | to<vector<string>>()};
    return *find_if(iota(1ll), [&](const Int) {
        for (auto &row : map)
            for (auto &cell : row) cell++;
        Int acc{0ll};
        for (const Int x : iota(0ll, 10ll))
            for (const Int y : iota(0ll, 10ll))
                acc += map[y][x] > '9' ? solution_helper(map, x, y) : 0ll;
        return acc == 100;
    });
}

int main() {
    const string_view str{
#include "../../input/input11.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay11"
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
