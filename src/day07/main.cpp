#include <iostream>
#include <string_view>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/min.hpp"
#include "range/v3/algorithm/nth_element.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/take.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::min;
using ranges::nth_element;
using ranges::to;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::take;
using ranges::views::transform;
using std::abs;
using std::atoll;
using std::cout;
using std::string_view;
using std::vector;

using Int = long long;

Int solution1(const string_view str) {
    auto subs{str | split(',') | transform([](const string_view pat) {
                  return atoll(pat.data());
              }) |
              to<vector<Int>>()};
    nth_element(subs, subs.begin() + (subs.size() / 2));
    const Int mid{subs[subs.size() / 2]};
    return accumulate(
        subs | transform([=](const Int n) { return abs(n - mid); }), 0ll);
}

Int solution2(const string_view str) {
    const auto subs{str | split(',') | transform([](const string_view pat) {
                        return atoll(pat.data());
                    }) |
                    to<vector<Int>>()};
    return min(iota(accumulate(subs, 0ll) / subs.size()) | take(2) |
               transform([&](const Int n) {
                   return accumulate(subs | transform([=](const Int i) {
                                         const Int dt{abs(n - i)};
                                         return dt * (dt + 1) / 2;
                                     }),
                                     0ll);
               }));
}

int main() {
    const string_view str{
#include "../../input/input07.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay07"
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
