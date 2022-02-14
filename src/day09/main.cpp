#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/all_of.hpp"
#include "range/v3/algorithm/sort.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::all_of;
using ranges::sort;
using ranges::to;
using ranges::views::enumerate;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::cout;
using std::greater;
using std::pair;
using std::string;
using std::string_view;
using std::vector;

using Int = long long;

const array<pair<Int, Int>, 4> neighbors{
    pair{0ll, -1ll}, {0ll, 1ll}, {1ll, 0ll}, {-1ll, 0ll}};

Int solution1(const string_view str) {
    const auto map{str | split('\n') | to<vector<string_view>>()};
    Int sum{0ll};
    for (const auto &[y, line] : map | enumerate) {
        for (const auto &[x, cell] : line | enumerate) {
            sum += all_of(neighbors,
                          [x = x, y = y, cell = cell,
                           &map](const pair<Int, Int> &pat) {
                              const auto [dx, dy] = pat;
                              // Implicit type conversion does not affect the
                              // result
                              return dx + x < 0 || dy + y < 0 || dx + x > 99 ||
                                     dy + y > 99 || map[dy + y][dx + x] > cell;
                          })
                       ? cell - '0' + 1ll
                       : 0ll;
        }
    }
    return sum;
}

Int solution2_helper(vector<string> &map, Int x, Int y) {
    map[y][x] = '9';
    return accumulate(
        neighbors | transform([=](const pair<Int, Int> &pat) {
            const auto [dx, dy] = pat;
            return pair{dx + x, dy + y};
        }),
        1ll, [&map](const Int acc, const pair<Int, Int> &pat) {
            const auto [x, y] = pat;
            return acc + (x < 0 || y < 0 || x > 99 || y > 99 || map[y][x] >= '9'
                              ? 0ll
                              : solution2_helper(map, x, y));
        });
}

Int solution2(const string_view str) {
    auto map{str | split('\n') | to<vector<string>>()};
    vector<Int> basins{};
    basins.reserve(512);
    for (const Int y : iota(0ll, 100ll))
        for (const Int x : iota(0ll, 100ll))
            if (map[y][x] < '9') basins.push_back(solution2_helper(map, x, y));
    sort(basins, greater<Int>());
    return basins[0] * basins[1] * basins[2];
}

int main() {
    const string_view str{
#include "../../input/input09.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay09"
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
