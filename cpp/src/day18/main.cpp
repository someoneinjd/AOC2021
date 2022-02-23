#include <algorithm>
#include <iostream>
#include <string_view>
#include <utility>
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
using std::max;
using std::pair;
using std::string_view;
using std::vector;

using Int = long long;
using Num = vector<pair<uint8_t, uint8_t>>;

void add_vector(Num &head, const Num &tail) {
    head.insert(head.cend(), tail.cbegin(), tail.cend());
    for (auto &[depth, _] : head) depth++;
}

void reduce(Num &nums, Int i) {
    for (const Int idx : iota(i, static_cast<Int>(nums.size()) - 1)) {
        if (nums[idx].first == 5) {
            const auto l{nums[idx].second};
            const auto r{nums[idx + 1].second};
            nums[idx] = {4, 0};
            nums.erase(nums.cbegin() + idx + 1);
            if (idx > 0) nums[idx - 1].second += l;
            nums[idx + 1].second += r;
            return reduce(nums, idx);
        }
    }
    for (const Int idx : iota(0u, nums.size())) {
        const auto [d, n]{nums[idx]};
        if (n >= 10) {
            nums[idx] = {d + 1, n / 2};
            nums.emplace(nums.cbegin() + idx + 1, d + 1, (n + 1) / 2);
            return reduce(nums, idx);
        }
    }
}

Int mag(auto &&i, const uint8_t depth, const Num &sf) {
    const auto res{
        3 * (sf[i].first == depth ? sf[i++].second : mag(i, depth + 1, sf))};
    return res +
           2 * (sf[i].first == depth ? sf[i++].second : mag(i, depth + 1, sf));
}

Int solution1(const string_view str) {
    auto nums{str | split('\n') | transform([](const string_view pat) {
                  Num res{};
                  res.reserve(pat.size() / 2);
                  uint8_t depth{0};
                  for (const uint8_t ch : pat) {
                      if (ch == '[') depth++;
                      if (ch == ']') depth--;
                      if (isdigit(ch)) res.emplace_back(depth, ch - '0');
                  }
                  return res;
              }) |
              to<vector>()};
    while (nums.size() > 1) {
        const auto tail{std::move(nums[1])};
        nums.erase(nums.cbegin() + 1);
        add_vector(nums[0], tail);
        reduce(nums[0], 0);
    }
    return mag(0, 1, nums[0]);
}

Int solution2(const string_view str) {
    auto nums{str | split('\n') | transform([](const string_view pat) {
                  Num res{};
                  res.reserve(pat.size() / 2);
                  uint8_t depth{0};
                  for (const uint8_t ch : pat) {
                      if (ch == '[') depth++;
                      if (ch == ']') depth--;
                      if (isdigit(ch)) res.emplace_back(depth, ch - '0');
                  }
                  return res;
              }) |
              to<vector>()};
    Int max_num{0ll};
    for (const Int i : iota(0, static_cast<Int>(nums.size()) - 1)) {
        for (const Int j : iota(i, static_cast<Int>(nums.size()))) {
            const auto &a{nums[i]};
            const auto &b{nums[j]};
            auto aa{nums[i]};
            auto bb{nums[j]};
            add_vector(aa, b);
            reduce(aa, 0);
            max_num = max(mag(0, 1, aa), max_num);
            add_vector(bb, a);
            reduce(bb, 0);
            max_num = max(mag(0, 1, bb), max_num);
        }
    }
    return max_num;
}

int main() {
    const string_view str{
#include "../../input/input18.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay18"
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
