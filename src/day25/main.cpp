#include <iostream>
#include <string_view>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::to;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::transform;
using std::cout;
using std::string_view;
using std::vector;

using Int = long long;

bool step(vector<vector<char>> &map, const Int dr, const Int dc) {
    const Int rows{static_cast<Int>(map.size())};
    const Int cols{static_cast<Int>(map[0].size())};
    vector new_map(rows, vector(cols, '.'));
    bool moved{false};
    for (const Int r : iota(0, rows)) {
        for (const Int c : iota(0, cols)) {
            if (map[r][c] == '>') {
                if (map[r][(c + dc) % cols] == '.') {
                    new_map[r][(c + dc) % cols] = '>';
                    moved = true;
                } else {
                    new_map[r][c] = '>';
                }
            } else if (map[r][c] == 'v') {
                if (map[(r + dr) % rows][c] == '.') {
                    new_map[(r + dr) % rows][c] = 'v';
                    moved = true;
                } else {
                    new_map[r][c] = 'v';
                }
            }
        }
    }
    map = std::move(new_map);
    return moved;
}

Int solution(const string_view str) {
    auto map{str | split('\n') | transform([](const string_view line) {
                 return line | to<vector>();
             }) |
             to<vector>()};
    Int res{0ll};
    while (true) {
        const bool res1{step(map, 0, 1)};
        const bool res2{step(map, 1, 0)};
        res++;
        if (!res1 && !res2) break;
    }
    return res;
}

int main() {
    const string_view str{
#include "../../input/input25.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution(str);
    const auto point2{system_clock::now()};
    cout << "\nDay25"
         << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 100000.
         << "ms\n";
#else
    cout << solution(str) << '\n' << "🎄" << '\n';
#endif
    return 0;
}
