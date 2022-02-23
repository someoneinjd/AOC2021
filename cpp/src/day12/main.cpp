#include <array>
#include <iostream>
#include <map>
#include <string_view>
#include <tuple>
#include <utility>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/find.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/enumerate.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/map.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::find;
using ranges::to;
using ranges::views::enumerate;
using ranges::views::filter;
using ranges::views::keys;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::cout;
using std::map;
using std::pair;
using std::string_view;
using std::tuple;
using std::vector;

using Int = long long;

Int solution1(const string_view str) {
    vector<uint8_t> uc{};
    map<string_view, uint8_t> id{{"start", 1}, {"end", 0}};
    map<uint8_t, vector<uint8_t>> maps{};
    const auto get_or_insert = [&](const string_view a) {
        const auto idx{static_cast<uint8_t>(id.size())};
        const auto [iter, no_contain]{id.try_emplace(a, idx)};
        if (!no_contain) return iter->second;
        if (a.front() <= 'Z') uc.push_back(idx);
        return idx;
    };
    const auto branch = [&](const uint8_t a, const uint8_t b) {
        vector<uint8_t> tmp{};
        tmp.reserve(6);
        const auto [iter, _]{maps.try_emplace(a, std::move(tmp))};
        if (b != 0) iter->second.push_back(b);
    };
    for (const string_view line : str | split('\n')) {
        const auto pos{line.find('-')};
        const auto a{get_or_insert(line.substr(0, pos))};
        const auto b{get_or_insert(line.substr(pos + 1))};
        branch(a, b);
        branch(b, a);
    }
    auto new_maps{maps | keys | filter([&](const uint8_t b) {
                      return find(uc, b) == uc.end();
                  }) |
                  transform([&](const uint8_t b) {
                      vector<uint8_t> res(12);
                      for (const auto ch : maps[b]) {
                          if (find(uc, ch) != uc.end())
                              for (const auto idx : maps[ch]) res[idx]++;
                          else
                              res[ch]++;
                      }
                      return pair{b, std::move(res)};
                  }) |
                  to<map>()};
    vector<tuple<uint8_t, uint8_t, Int>> todo{{0, 1, 1ll}};
    array<Int, 12> to{};
    Int res{0ll};
    to.fill(1ll);
    while (!todo.empty()) {
        const auto [a, b, s]{todo.back()};
        todo.pop_back();
        to[b] = a;
        for (const auto [idx, _] :
             new_maps[a] | enumerate |
                 filter([](const auto &pair) { return pair.second > 0; })) {
            if (idx == 1)
                res += s * new_maps[a][idx];
            else {
                const auto pos{find(to, idx) - to.begin()};
                if (!(idx != 0 && pos >= 2 && pos <= b))
                    todo.emplace_back(idx, b + 1, s * new_maps[a][idx]);
            }
        }
    }
    return res;
}

Int solution2(const string_view str) {
    vector<uint8_t> uc{};
    map<string_view, uint8_t> id{{"start", 1}, {"end", 0}};
    map<uint8_t, vector<uint8_t>> maps{};
    const auto get_or_insert = [&](const string_view a) {
        const auto idx{static_cast<uint8_t>(id.size())};
        const auto [iter, no_contain]{id.try_emplace(a, idx)};
        if (!no_contain) return iter->second;
        if (a.front() <= 'Z') uc.push_back(idx);
        return idx;
    };
    const auto branch = [&](const uint8_t a, const uint8_t b) {
        vector<uint8_t> tmp{};
        tmp.reserve(6);
        const auto [iter, _]{maps.try_emplace(a, std::move(tmp))};
        if (b != 0) iter->second.push_back(b);
    };
    for (const string_view line : str | split('\n')) {
        const auto pos{line.find('-')};
        const auto a{get_or_insert(line.substr(0, pos))};
        const auto b{get_or_insert(line.substr(pos + 1))};
        branch(a, b);
        branch(b, a);
    }
    auto new_maps{maps | keys | filter([&](const uint8_t b) {
                      return find(uc, b) == uc.end();
                  }) |
                  transform([&](const uint8_t b) {
                      vector<uint8_t> res(12);
                      for (const auto ch : maps[b]) {
                          if (find(uc, ch) != uc.end())
                              for (const auto idx : maps[ch]) res[idx]++;
                          else
                              res[ch]++;
                      }
                      return pair{b, std::move(res)};
                  }) |
                  to<map>()};
    vector<tuple<uint8_t, uint8_t, bool, Int>> todo{{0, 1, true, 1ll}};
    array<Int, 12> to{};
    Int res{0ll};
    to.fill(1ll);
    while (!todo.empty()) {
        const auto [a, b, t, s]{todo.back()};
        todo.pop_back();
        to[b] = a;
        for (const auto [idx, _] :
             new_maps[a] | enumerate |
                 filter([](const auto &pair) { return pair.second > 0; })) {
            if (idx == 1)
                res += s * new_maps[a][idx];
            else {
                const auto pos{find(to, idx) - to.begin()};
                const auto visited = !(idx != 0 && pos >= 2 && pos <= b);
                if (t || visited)
                    todo.emplace_back(idx, b + 1, t && visited,
                                      s * new_maps[a][idx]);
            }
        }
    }
    return res;
}

int main() {
    const string_view str{
#include "../../input/input12.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay12"
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
