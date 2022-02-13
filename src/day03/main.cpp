#include <iostream>
#include <string_view>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/reverse.hpp"
#include "range/v3/view/transform.hpp"

#ifdef AOC_BENCH
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::count_if;
using ranges::to;
using ranges::views::enumerate;
using ranges::views::iota;
using ranges::views::reverse;
using ranges::views::split;
using ranges::views::transform;
using std::cout;
using std::erase_if;
using std::string_view;
using std::strtoll;
using std::vector;

using Int = long long;

Int solution1(const string_view str) {
    auto tmp = accumulate(
        str | split('\n') | transform([](const string_view pat) {
            return strtoll(pat.data(), nullptr, 2);
        }),
        vector(12, 0ll), [](auto &&count, const Int bits) {
            return count | enumerate | transform([=](const auto &pair) {
                       return pair.second +
                              ((bits & 1 << pair.first) >> pair.first);
                   }) |
                   to<vector<Int>>();
        });
    auto res = accumulate(tmp | enumerate | transform([](const auto &pair) {
                              return static_cast<Int>(pair.second >= 500)
                                     << pair.first;
                          }),
                          0ull);
    return res * (~res & ((1 << 12) - 1));
}

Int solution2(const string_view str) {
    auto num1{str | split('\n') | transform([](const string_view pat) {
                  return strtoull(pat.data(), nullptr, 2);
              }) |
              to<vector<Int>>()};
    auto num2{num1};
    for (const Int i : iota(0ull, 12ull) | reverse) {
        const Int one1{count_if(num1, [=](const Int n) {
                           return (n & (1 << i)) > 0;
                       }) >= (num1.size() + 1) / 2};
        const Int one2{count_if(num2, [=](const Int n) {
                           return (n & (1 << i)) > 0;
                       }) >= (num2.size() + 1) / 2};
        erase_if(num1,
                 [=](const Int n) { return ((n & (1 << i)) > 0) == one1; });
        erase_if(num2,
                 [=](const Int n) { return ((n & (1 << i)) > 0) != one2; });
    }
    return num1[0] * num2[0];
}

int main() {
    const string_view str{
#include "../../input/input03.txt"
    };
#ifdef AOC_BENCH
    const auto point1{system_clock::now()};
    const Int res1{solution1(str)};
    const auto point2{system_clock::now()};
    const Int res2{solution2(str)};
    const auto point3{system_clock::now()};
    cout << res1 << '\n'
         << res2 << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 1000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 1000.
         << "ms\n";
#else
    cout << solution1(str) << '\n' << solution2(str) << '\n';
#endif
    return 0;
}
