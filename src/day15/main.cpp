#include <array>
#include <iostream>
#include <limits>
#include <set>
#include <string_view>
#include <utility>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::to;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::cout;
using std::numeric_limits;
using std::pair;
using std::set;
using std::string_view;
using std::vector;

using Int = long long;

const static array<pair<Int, Int>, 4> NEXT{{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};

template <typename F>
Int dijkstra(F get_neighbors, const Int width, const Int height) {
    const auto size{width * height};
    set<pair<Int, Int>> setds{};
    vector<Int> dist(size, numeric_limits<Int>::max());
    setds.emplace(0, 0);
    dist[0] = 0;
    while (!setds.empty()) {
        const auto [_, relay]{*setds.begin()};
        setds.extract(setds.begin());
        const auto neighbors{get_neighbors(relay / width, relay % width)};
        for (const auto [node, weight] : neighbors) {
            if (dist[node] > dist[relay] + weight) {
                if (dist[node] != numeric_limits<Int>::max())
                    setds.extract({dist[node], node});
                dist[node] = dist[relay] + weight;
                setds.emplace(dist[node], node);
            }
        }
    }
    return dist.back();
}

Int solution1(const string_view str) {
    const auto map{str | split('\n') | transform([](const string_view pat) {
                       return pat | transform([](const char ch) -> Int {
                                  return ch - '0';
                              }) |
                              to<vector>();
                   }) |
                   to<vector>()};
    const auto width{static_cast<Int>(map[0].size())};
    const auto height{static_cast<Int>(map.size())};
    return dijkstra(
        [&](const Int x, const Int y) {
            vector<pair<Int, Int>> res{};
            res.reserve(4);
            for (const auto [dx, dy] : NEXT) {
                if (x + dx < 0 || x + dx >= height || y + dy < 0 ||
                    y + dy >= width)
                    continue;
                res.emplace_back(y + dy + (x + dx) * width,
                                 map[x + dx][y + dy]);
            }
            return res;
        },
        width, height);
}

Int solution2(const string_view str) {
    const auto map{str | split('\n') | transform([](const string_view pat) {
                       return pat | transform([](const char ch) -> Int {
                                  return ch - '0';
                              }) |
                              to<vector>();
                   }) |
                   to<vector>()};
    const auto width{static_cast<Int>(map[0].size())};
    const auto height{static_cast<Int>(map.size())};
    const auto s{static_cast<Int>(map.size())};
    return dijkstra(
        [&](const Int x, const Int y) {
            vector<pair<Int, Int>> res{};
            res.reserve(4);
            for (const auto [dx, dy] : NEXT) {
                const auto next_x{x + dx};
                const auto next_y{y + dy};
                if (next_x < 0 || next_y < 0 || next_x >= 5 * height ||
                    next_y >= 5 * width)
                    continue;
                res.emplace_back(next_y + next_x * 5ll * width,
                                 (map[next_x % 100][next_y % 100] +
                                  next_x / 100 + next_y / 100 - 1) %
                                         9 +
                                     1);
            }
            return res;
        },
        width * 5ll, height * 5ll);
}

int main() {
    const string_view str{
#include "../../input/input15.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay15"
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
