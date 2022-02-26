#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <string_view>
#include <utility>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/count.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/view/iota.hpp"
#include "range/v3/view/transform.hpp"

#ifdef BENCHMARK
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::count;
using ranges::views::iota;
using ranges::views::split;
using ranges::views::transform;
using std::cout;
using std::make_move_iterator;
using std::max;
using std::min;
using std::multiplies;
using std::numeric_limits;
using std::pair;
using std::string_view;
using std::swap;
using std::vector;

using Int = long long;
using Num = vector<Int>;

Num parse(const string_view str) {
    // No regex !!!
    Num res(6);
    const char *begin{str.data()};
    char *end{nullptr};
    for (auto &i : res) {
        while (!isdigit(*begin) && *begin != '-') begin++;
        i = strtoll(begin, &end, 10);
        begin = end;
    }
    return res;
}

Num intersect(const Num &a, const Num &b) {
    const vector res{max(a[0], b[0]), max(a[1], b[1]), max(a[2], b[2]),
                     min(a[3], b[3]), min(a[4], b[4]), min(a[5], b[5])};
    for (const Int i : iota(0, 3))
        if (res[i] > res[i + 3]) return {};
    return res;
}

void cut(const Num &boxx, const Num &cut, vector<Num> &new_vec) {
    if (cut[0] > boxx[0])
        new_vec.push_back(
            {boxx[0], boxx[1], boxx[2], cut[0] - 1, boxx[4], boxx[5]});
    if (cut[3] < boxx[3])
        new_vec.push_back(
            {cut[3] + 1, boxx[1], boxx[2], boxx[3], boxx[4], boxx[5]});
    if (cut[4] < boxx[4])
        new_vec.push_back(
            {cut[0], cut[4] + 1, boxx[2], cut[3], boxx[4], boxx[5]});
    if (cut[1] > boxx[1])
        new_vec.push_back(
            {cut[0], boxx[1], boxx[2], cut[3], cut[1] - 1, boxx[5]});
    if (cut[2] > boxx[2])
        new_vec.push_back(
            {cut[0], cut[1], boxx[2], cut[3], cut[4], cut[2] - 1});
    if (cut[5] < boxx[5])
        new_vec.push_back(
            {cut[0], cut[1], cut[5] + 1, cut[3], cut[4], boxx[5]});
}

Int solution1(const string_view str) {
    vector<pair<bool, Num>> info{};
    info.reserve(count(str, '\n'));
    for (const string_view line : str | split('\n')) {
        auto tmp{parse(line)};
        bool continue_flag{false};
        for (const Int i : iota(0, 3)) {
            tmp[2 * i] = max(tmp[2 * i], -50ll);
            tmp[2 * i + 1] = min(tmp[2 * i + 1], 50ll);
            if (!(tmp[2 * i] <= 50 && tmp[2 * i + 1] >= -50)) {
                continue_flag = true;
                break;
            }
        }
        if (continue_flag) continue;
        swap(tmp[1], tmp[2]);
        swap(tmp[2], tmp[4]);
        swap(tmp[3], tmp[4]);
        info.emplace_back(line.substr(0, line.find(' ')) == "on",
                          std::move(tmp));
    }
    auto res{0ll};
    vector<Num> boxes{};
    boxes.reserve(64);
    for (const auto [on, info] : info) {
        if (!boxes.empty()) {
            decltype(boxes) new_vec{};
            for (auto &boxx : boxes) {
                const auto tmp{intersect(info, boxx)};
                if (tmp.size() == 0) continue;
                cut(boxx, tmp, new_vec);
                boxx = {0,
                        0,
                        0,
                        numeric_limits<Int>::min(),
                        numeric_limits<Int>::min(),
                        numeric_limits<Int>::min()};
                res -= accumulate(iota(0, 3) | transform([&](const Int c) {
                                      return tmp[3 + c] - tmp[c] + 1;
                                  }),
                                  1ll, multiplies<Int>());
            }
            boxes.insert(boxes.cend(), make_move_iterator(new_vec.begin()),
                         make_move_iterator(new_vec.end()));
        }
        if (on) {
            res += accumulate(iota(0, 3) | transform([&info = info](const Int c) {
                                  return info[3 + c] - info[c] + 1;
                              }),
                              1ll, multiplies<Int>());
            boxes.emplace_back(std::move(info));
        }
    }
    return res;
}

Int solution2(const string_view str) {
    vector<pair<bool, Num>> info{};
    info.reserve(count(str, '\n'));
    for (const string_view line : str | split('\n')) {
        auto tmp{parse(line)};
        swap(tmp[1], tmp[2]);
        swap(tmp[2], tmp[4]);
        swap(tmp[3], tmp[4]);
        info.emplace_back(line.substr(0, line.find(' ')) == "on",
                          std::move(tmp));
    }
    auto res{0ll};
    vector<Num> boxes{};
    boxes.reserve(64);
    for (const auto [on, info] : info) {
        if (!boxes.empty()) {
            decltype(boxes) new_vec{};
            for (auto &boxx : boxes) {
                const auto tmp{intersect(info, boxx)};
                if (tmp.size() == 0) continue;
                cut(boxx, tmp, new_vec);
                boxx = {0,
                        0,
                        0,
                        numeric_limits<Int>::min(),
                        numeric_limits<Int>::min(),
                        numeric_limits<Int>::min()};
                res -= accumulate(iota(0, 3) | transform([&](const Int c) {
                                      return tmp[3 + c] - tmp[c] + 1;
                                  }),
                                  1ll, multiplies<Int>());
            }
            boxes.insert(boxes.cend(), make_move_iterator(new_vec.begin()),
                         make_move_iterator(new_vec.end()));
        }
        if (on) {
            res += accumulate(iota(0, 3) | transform([&info = info](const Int c) {
                                  return info[3 + c] - info[c] + 1;
                              }),
                              1ll, multiplies<Int>());
            boxes.emplace_back(std::move(info));
        }
    }
    return res;
}

int main() {
    const string_view str{
#include "../../input/input22.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay22"
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
