#include <iostream>
#include <string_view>
#include <tuple>

#include "../better_split.hpp"
#include "range/v3/algorithm/fold_left.hpp"

#ifdef AOC_BENCH
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::fold_left;
using ranges::views::split;
using std::atoll;
using std::cout;
using std::string_view;
using std::tuple;

using Int = long long;

Int solution1(const string_view str) {
    const auto [f, d] =
        fold_left(str | split('\n'), tuple{0ll, 0ll},
                  [](const auto &res, const string_view cmd) {
                      auto [f, d] = res;
                      const auto step_num{
                          atoll(cmd.substr(cmd.find_first_of(' ') + 1).data())};
                      if (cmd.starts_with("forward"))
                          f += step_num;
                      else if (cmd.starts_with("down"))
                          d += step_num;
                      else
                          d -= step_num;
                      return tuple{f, d};
                  });
    return f * d;
}

Int solution2(const string_view str) {
    const auto [f, d, _] =
        fold_left(str | split('\n'), tuple{0ll, 0ll, 0ll},
                  [](const auto &res, const string_view cmd) {
                      auto [f, d, a] = res;
                      const auto step_num{
                          atoll(cmd.substr(cmd.find_first_of(' ') + 1).data())};
                      if (cmd.starts_with("forward"))
                          f += step_num, d += a * step_num;
                      else if (cmd.starts_with("down"))
                          a += step_num;
                      else
                          a -= step_num;
                      return tuple{f, d, a};
                  });
    return f * d;
}

int main() {
    const string_view str{
#include "../../input/input02.txt"
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
