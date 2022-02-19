#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::to;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::count;
using std::cout;
using std::string_view;
using std::vector;

using Int = long long;

template <Int N>
Int solution(const string_view str) {
    const auto pos{str.find("\n\n")};
    const auto size{count(str.cbegin() + pos + 2, str.cend(), '\n') + 1};
    const auto key{str.substr(0, pos) | transform([](const char ch) {
                       return static_cast<uint8_t>(ch == '#');
                   }) |
                   to<vector>()};
    vector map(N, vector<uint8_t>(size + 2 * N));
    map.reserve(128);
    for (const string_view line : str.substr(pos + 2) | split('\n')) {
        vector<uint8_t> res(N);
        res.reserve(128);
        for (const char ch : line) res.push_back(ch == '#');
        for (const Int _ : iota(0, N)) res.push_back(0);
        map.emplace_back(std::move(res));
    }
    for (const Int _ : iota(0, N)) map.emplace_back(size + 2 * N);
    auto other{map};
    for (const Int i : iota(0, N)) {
        const Int out{i % 2 && key[0]};
        for (const Int y : iota(0, static_cast<Int>(map.size()))) {
            auto i{0ll};
            const array rows{
                y == 0 ? vector<uint8_t>{} : map[y - 1], map[y],
                y + 1 == map.size() ? vector<uint8_t>{} : map[y + 1]};
            for (const auto& row : rows) {
                if (row.empty())
                    i = 7 * out | i << 3;
                else {
                    auto res{0ll};
                    for (const Int c :
                         iota(-1ll, 2ll) | transform([&](const Int x) {
                             if (x == -1) return out;
                             return static_cast<Int>(row[x]);
                         })) {
                        res = res << 1 | c;
                    }
                    i = res | i << 3;
                }
            }
            for (const Int x : iota(0, static_cast<Int>(map[0].size()))) {
                const Int tmp{
                    (x + 2 < rows[0].size() ? rows[0][x + 1] : out) << 6 |
                    (x + 2 < rows[1].size() ? rows[1][x + 1] : out) << 3 |
                    (x + 2 < rows[2].size() ? rows[2][x + 1] : out)};
                i = i & 0b110110110 | tmp;
                other[y][x] = key[i];
                i <<= 1;
            }
        }
        map.swap(other);
    }
    return accumulate(map | transform([](const auto& r) {
                          return count(r.cbegin(), r.cend(), 1);
                      }),
                      0ll);
}

int main() {
    const string_view str{
#include "../../input/input20.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<2>(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<50>(str);
    const auto point3{system_clock::now()};
    cout << "\nDay20"
         << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 100000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 100000.
         << "ms\n";
#else
    cout << solution<2>(str) << '\n' << solution<50>(str) << '\n';
#endif
    return 0;
}
