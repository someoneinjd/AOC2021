#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/slice.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::views::enumerate;
using ranges::views::iota;
using ranges::views::slice;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::cout;
using std::max;
using std::min;
using std::string_view;
using std::vector;

using Int = long long;

const static array distr{1ll, 3ll, 6ll, 7ll, 6ll, 3ll, 1ll};

vector<Int> score(const Int pos) {
    vector tab(11, vector(11, vector(22, 0ll)));
    tab[0][pos][0] = 1;
    for (const Int i : iota(1, 11)) {
        for (const Int j : iota(1, 11)) {
            for (const Int k : iota(0, 21)) {
                for (const auto [idx, val] : distr | enumerate) {
                    const Int q{static_cast<Int>(j + idx + 2) % 10 + 1};
                    const Int v{min(q + k, 21ll)};
                    tab[i][q][v] += val * tab[i - 1][j][k];
                }
            }
        }
    }
    vector out(22, 0ll);
    for (const auto [idx, tab] : tab | enumerate)
        for (const auto tab : tab | slice(1, 11)) {
            for (const auto tab : tab | slice(0, 21)) out[idx + 11] += tab;
            out[idx] += tab[21];
        }
    return out;
}

Int solution1(const string_view str) {
    array pos{0ll, 0ll};
    array score{0ll, 0ll};
    for (const auto [idx, nums] :
         str | split('\n') | transform([](const string_view line) {
             return atoll(line.data() + line.size() - 1) - 1ll;
         }) | enumerate)
        pos[idx] = nums;
    auto generator{[ret = 0ll]() mutable { return 3 * (ret++ * 3 + 2); }};
    for (const Int turn : iota(0)) {
        for (const Int p : iota(0, 2)) {
            pos[p] = (pos[p] + generator()) % 10;
            score[p] += pos[p] + 1;
            if (score[p] >= 1000)
                return score[(p + 1) % 2] * (turn * 6 + ((p + 1) * 3));
        }
    }
}

Int solution2(const string_view str) {
    array pos{0ll, 0ll};
    for (const auto [idx, nums] :
         str | split('\n') | transform([](const string_view line) {
             return atoll(line.data() + line.size() - 1);
         }) | enumerate)
        pos[idx] = nums;
    const auto first{score(pos[0])};
    const auto second{score(pos[1])};
    return max(accumulate(iota(1ll, 11ll) | transform([&](const Int i) {
                              return first[i] * second[i + 10];
                          }),
                          0ll),
               accumulate(iota(1ll, 11ll) | transform([&](const Int i) {
                              return first[i + 10] * second[i - 1];
                          }),
                          0ll));
}

int main() {
    const string_view str{
#include "../../input/input21.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay21"
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
