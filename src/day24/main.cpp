#include <array>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <string_view>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/count.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/iota.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::count;
using ranges::to;
using ranges::views::iota;
using ranges::views::split;
using std::cout;
using std::hash;
using std::numeric_limits;
using std::pow;
using std::size_t;
using std::string_view;
using std::tuple;
using std::unordered_set;
using std::vector;

using Int = long long;
using Seq = vector<tuple<Int, Int, Int>>;
using Rng = vector<Int>;
using Par = tuple<Int, Int>;

struct hash_pair {
    std::size_t operator()(const Par &pair) const {
        const hash<Int> hasher{};
        size_t seed = 0;
        const auto [x, y]{pair};
        seed ^= hasher(x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};

template <Int Begin, Int End, Int Step>
Int find_modelnum(auto &&cache, const Seq &blocks, const Int block,
                  const Int z) {
    if (block == blocks.size()) return z == 0 ? 0 : numeric_limits<Int>::min();
    if (z > pow(26ll, 14ll - block) || cache.contains({block, z}))
        return numeric_limits<Int>::min();
    const auto [a, b, c]{blocks[block]};
    for (Int i{Begin}; i != End; i += Step) {
        const auto new_z{z % 26 + b == i ? z / a : z / a * 26 + i + c};
        if (const auto res{find_modelnum<Begin, End, Step>(cache, blocks,
                                                           block + 1, new_z)};
            res != numeric_limits<Int>::min())
            return i * pow(10ll, 13ll - block) + res;
    }
    cache.emplace(block, z);
    return numeric_limits<Int>::min();
}

template <Int Begin, Int End, Int Step>
Int solution(const string_view str) {
    const Int num{count(str, '\n') + 1ll};
    const auto lines{str | split('\n') | to<vector<string_view>>()};
    Seq blocks{};
    for (const Int i : iota(0, num / 18))
        blocks.emplace_back(atoll(lines[18 * i + 4].substr(6).data()),
                            atoll(lines[18 * i + 5].substr(6).data()),
                            atoll(lines[18 * i + 15].substr(6).data()));
    return find_modelnum<Begin, End, Step>(unordered_set<Par, hash_pair>{},
                                           blocks, 0, 0);
}

int main() {
    const string_view str{
#include "../../input/input24.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<9, -1, -1>(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution<1, 10, 1>(str);
    const auto point3{system_clock::now()};
    cout << "\nDay24"
         << "\nElapsed Time 1:\t"
         << duration_cast<microseconds>(point2 - point1).count() / 100000.
         << "ms\n"
         << "Elapsed Time 2:\t"
         << duration_cast<microseconds>(point3 - point2).count() / 100000.
         << "ms\n";
#else
    cout << solution<9, -1, -1>(str) << '\n' << solution<1, 10, 1>(str) << '\n';
#endif
    return 0;
}
