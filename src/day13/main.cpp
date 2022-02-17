#include <array>
#include <iostream>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/sort.hpp"
#include "range/v3/algorithm/unique.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::sort;
using ranges::to;
using ranges::unique;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::atoll;
using std::cout;
using std::pair;
using std::string;
using std::string_view;
using std::vector;

using Int = long long;

Int solution1(const string_view str) {
    const auto pos{str.find("\n\n")};
    const auto coords{str.substr(0, pos)};
    const auto folds{str.substr(pos + 2)};
    const auto slice{folds.substr(11, folds.find('\n') - 11)};
    const auto c{slice.front()};
    const auto i{atoll(slice.substr(slice.find('=') + 1).data())};
    vector<pair<Int, Int>> new_coords{};
    new_coords.reserve(64);
    auto cnt{0ll};
    auto rng = coords | split('\n') | transform([&](const string_view pat) {
                   const auto pos{pat.find(',')};
                   return pair{atoll(pat.substr(0, pos).data()),
                               atoll(pat.substr(pos + 1).data())};
               });
    if (c == 'x') {
        for (const auto [x, y] : rng)
            if (x > i)
                new_coords.emplace_back(2 * i - x, y);
            else if (x < i)
                new_coords.emplace_back(x, y);
    } else if (c == 'y') {
        for (const auto [x, y] : rng)
            if (y > i)
                new_coords.emplace_back(x, 2 * i - y);
            else if (y < i)
                new_coords.emplace_back(x, y);
    }
    sort(new_coords);
    return unique(new_coords) - new_coords.begin();
}

string solution2(const string_view str) {
    const auto pos{str.find("\n\n")};
    const auto coords{str.substr(0, pos)};
    const auto folds{
        str.substr(pos + 2) | split('\n') |
        transform([](const string_view pat) {
            return pair{pat[11], atoll(pat.substr(pat.find('=') + 1).data())};
        }) |
        to<vector>()};
    array<Int, 6> map{};
    for (const auto [x, y] :
         coords | split('\n') | transform([&](const string_view pat) {
             const auto pos{pat.find(',')};
             auto x = atoll(pat.substr(0, pos).data());
             auto y = atoll(pat.substr(pos + 1).data());
             for (const auto [c, i] : folds) {
                 if (c == 'x') {
                     if (x == i) return pair{-1ll, -1ll};
                     if (x > i) x = 2 * i - x;
                 } else if (c == 'y') {
                     if (y == i) return pair{-1ll, -1ll};
                     if (y > i) y = 2 * i - y;
                 }
             }
             return pair{x, y};
         })) {
        if (x < 0) continue;
        map[y] |= 1ull << x;
    }
    string res{};
    res.reserve(128);
    for (const auto row : map) {
        for (const auto x : iota(0, 40))
            res.push_back(' ' +
                          3 * static_cast<uint8_t>((row & 1ull << x) >> x));
        res.push_back('\n');
    }
    return res;
}

int main() {
    const string_view str{
#include "../../input/input13.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay13"
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
