#include <cmath>
#include <iostream>
#include <string_view>

#include "../better_split.hpp"
#include "range/v3/algorithm/count_if.hpp"
#include "range/v3/algorithm/find_if.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"

#ifdef AOC_BENCH
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::count_if;
using ranges::find_if;
using ranges::views::enumerate;
using ranges::views::join;
using ranges::views::split;
using ranges::views::transform;
using std::cout;
using std::pow;
using std::string_view;

using Int = long long;

Int solution1(const string_view str) {
    return count_if(str | split('\n') | transform([](const string_view pat) {
                        return pat.substr(pat.find('|') + 2) | split(' ');
                    }) | join,
                    [](const string_view pat) {
                        return pat.size() == 7 ||
                               (pat.size() < 5 && pat.size() > 1);
                    });
}

Int solution2(const string_view str) {
    return accumulate(
        str | split('\n') | transform([](const string_view pat) {
            const auto pos{pat.find('|')};
            auto input{pat.substr(0, pos - 1) | split(' ')};
            const string_view two{*find_if(
                input, [](const string_view pat) { return pat.size() == 2; })};
            const string_view four{*find_if(
                input, [](const string_view pat) { return pat.size() == 4; })};
            return accumulate(
                pat.substr(pos + 2) | split(' ') |
                    transform([=](const string_view pat) {
                        if (pat.size() == 2)
                            return 1ll;
                        else if (pat.size() == 3)
                            return 7ll;
                        else if (pat.size() == 4)
                            return 4ll;
                        else if (pat.size() == 7)
                            return 8ll;
                        else {
                            const auto tmp1{count_if(pat, [=](const char ch) {
                                return two.find(ch) != string_view::npos;
                            })};
                            const auto tmp2{count_if(pat, [=](const char ch) {
                                return four.find(ch) != string_view::npos;
                            })};
                            if (pat.size() == 5)
                                return tmp2 == 2 ? 2ll : -2 * tmp1 + 7;
                            else
                                return tmp1 == 1 ? 6ll : 9 * tmp2 - 27;
                        }
                    }) |
                    enumerate,
                0ll, [](const Int acc, const auto &pair) {
                    return acc + pair.second * pow(10ll, 3ll - pair.first);
                });
        }),
        0ll);
}

int main() {
    const string_view str{
#include "../../input/input08.txt"
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
