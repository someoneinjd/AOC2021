#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

#include "range/v3/view/iota.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::views::iota;
using std::array;
using std::cout;
using std::min;
using std::minmax;
using std::string_view;

using Int = long long;

array<Int, 4> parse(const string_view str) {
    // No regex !!!
    array<Int, 4> res{};
    const char *begin{str.data()};
    char *end{nullptr};
    for (auto &i : res) {
        while (!isdigit(*begin) && *begin != '-') begin++;
        i = strtoll(begin, &end, 10);
        begin = end;
    }
    return res;
}

Int solution1(const string_view str) {
    const auto nums{parse(str.substr(str.find('=') + 1))};
    const Int n{-min(nums[2], nums[3]) - 1ll};
    return n * (n + 1) / 2ll;
}

Int solution2(const string_view str) {
    const auto nums{parse(str.substr(str.find('=') + 1))};
    const auto [x_min, x_max]{minmax(nums[0], nums[1])};
    const auto [y_min, y_max]{minmax(nums[2], nums[3])};
    const auto reaches{[x_min = x_min, x_max = x_max, y_min = y_min,
                        y_max = y_max](Int vx, Int vy) {
        Int posx = 0, posy = 0;
        while (true) {
            posx += vx;
            posy += vy;
            if (posx >= x_min && posx <= x_max && posy >= y_min &&
                posy <= y_max)
                return true;
            vy--;
            if (vx > 0) vx--;
            if (posy < y_min || posx > x_max || vx == 0 && posx < x_min) break;
        }
        return false;
    }};
    Int res{0ll};
    for (Int vy : iota(y_min, -y_min))
        for (Int vx : iota(1, x_max + 1)) res += reaches(vx, vy);
    return res;
}

int main() {
    const string_view str{
#include "../../input/input17.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay17"
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
