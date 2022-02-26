#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::any_of;
using ranges::to;
using ranges::views::enumerate;
using ranges::views::filter;
using ranges::views::iota;
using ranges::views::join;
using ranges::views::split;
using ranges::views::transform;
using std::atoll;
using std::cout;
using std::pair;
using std::string_view;
using std::unordered_map;
using std::vector;

using Int = long long;
using namespace std::literals::string_view_literals;

template <bool F>
Int solution(const string_view str) {
    const auto pos{str.find("\n\n")};
    const auto nums{str.substr(0, pos)};
    const auto boards{str.substr(pos + 2)};
    auto new_boards{
        boards | split("\n\n"sv) | transform([](const string_view pat) {
            return pair{
                pat | split('\n') | transform([](const string_view pat) {
                    return pat | split(' ') | filter([](const string_view pat) {
                               return pat.size() != 0;
                           });
                }) | join |
                    enumerate | transform([](const auto &pat) {
                        return pair{atoll(pat.second.data()), pat.first};
                    }) |
                    to<unordered_map<Int, Int>>(),
                0ll};
        }) |
        to<vector<pair<unordered_map<Int, Int>, Int>>>()};
    decltype(new_boards[0].first) board{};
    Int mark{0ll}, num{0ll};
    bool break_flag{false};
    for (const Int n : nums | split(',') | transform([](const string_view pat) {
                           return atoll(pat.data());
                       })) {
        for (auto &[b, m] : new_boards) {
            if (!b.contains(n)) continue;
            m |= 1 << b[n];
            if (any_of(iota(0ll, 5ll), [m = m](const Int i) {
                    return (m >> (i * 5) & 0b11111) == 0b11111 ||
                           (m >> i & 0b100001000010000100001) ==
                               0b100001000010000100001;
                })) {
                mark = m;
                num = n;
                board = std::move(b);
                break_flag = F;
            }
        }
        if (break_flag) break;
    }
    return accumulate(board | transform([=](const auto &pat) {
                          const auto [n, i] = pat;
                          return (mark >> i & 1 ^ 1) * n * num;
                      }),
                      0ll);
}

int main() {
    const string_view str{
#include "../../input/input04.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<true>(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<false>(str);
    const auto point3{system_clock::now()};
    cout << "\nDay04"
         << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 100000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 100000.
         << "ms\n";
#else
    cout << solution<true>(str) << '\n' << solution<false>(str) << '\n';
#endif
    return 0;
}
