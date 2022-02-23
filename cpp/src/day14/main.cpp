#include <array>
#include <iostream>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/lower_bound.hpp"
#include "range/v3/algorithm/minmax.hpp"
#include "range/v3/algorithm/sort.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/sliding.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::lower_bound;
using ranges::minmax;
using ranges::sort;
using ranges::to;
using ranges::views::filter;
using ranges::views::iota;
using ranges::views::sliding;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::cout;
using std::get;
using std::pair;
using std::string_view;
using std::tuple;
using std::vector;

using Int = long long;

template <Int N>
Int solution(const string_view str) {
    const auto pos{str.find("\n\n")};
    const auto base{str.substr(0, pos)};
    auto rules{
        str.substr(pos + 2) | split('\n') |
        transform([](const string_view pat) {
            const auto pos{pat.find(" -> ")};
            return pair{array{pat[0], pat[1]}, array{pat[0], pat[pos + 4]}};
        }) |
        to<vector>()};
    const auto cmp{
        [](const auto &fst, const auto &snd) { return fst.first < snd.first; }};
    sort(rules, cmp);
    const auto rule{
        rules | transform([&](const auto &pat) {
            return tuple{
                pat.first,
                lower_bound(rules, pair{pat.second, array{0, 0}}, cmp) -
                    rules.begin(),
                lower_bound(
                    rules,
                    pair{array{pat.second[1], pat.first[1]}, array{0, 0}},
                    cmp) -
                    rules.begin()};
        }) |
        to<vector>()};
    vector<Int> num(rule.size());
    vector<Int> next(rule.size());
    for (const auto &pat : base | sliding(2)) {
        num[lower_bound(rule, tuple{array{pat.front(), pat.back()}, 0, 0},
                        [](const auto &fst, const auto &snd) {
                            return get<0>(fst) < get<0>(snd);
                        }) -
            rule.begin()]++;
    }
    for (const auto _ : iota(0ll, N)) {
        for (const auto idx : iota(0u, num.size())) {
            next[get<1>(rule[idx])] += num[idx];
            next[get<2>(rule[idx])] += num[idx];
            num[idx] = 0;
        }
        next.swap(num);
    }
    array<Int, 26> occur{};
    occur[base.back() - 'A'] += 1;
    for (const auto idx : iota(0u, num.size()))
        occur[get<0>(rule[idx])[0] - 'A'] += num[idx];
    const auto [min, max]{
        minmax(occur | filter([](const Int n) { return n != 0; }))};
    return max - min;
}

int main() {
    const string_view str{
#include "../../input/input14.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<10>(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<40>(str);
    const auto point3{system_clock::now()};
    cout << "\nDay14"
         << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 100000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 100000.
         << "ms\n";
#else
    cout << solution<10>(str) << '\n' << solution<40>(str) << '\n';
#endif
    return 0;
}
