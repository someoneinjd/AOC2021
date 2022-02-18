#include <algorithm>
#include <functional>
#include <iostream>
#include <string_view>
#include <vector>

#include "range/v3/algorithm/max.hpp"
#include "range/v3/algorithm/min.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/chunk.hpp"
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
using ranges::views::chunk;
using ranges::views::iota;
using ranges::views::transform;
using std::cout;
using std::min;
using std::multiplies;
using std::string_view;
using std::vector;

using Int = long long;

struct Bits {
    const uint8_t *base;
    Int len;
    Int pos;
    Bits(const uint8_t *base, Int len, Int pos)
        : base{base}, len{len}, pos{pos} {}
    Bits(const vector<uint8_t> &v)
        : base{v.data()}, len{static_cast<Int>(v.size()) * 8}, pos{0} {}
    void skip(Int count) { pos += count; }
    Int take(Int count) {
        Int out{0ll};
        while (count > 0) {
            const auto left{8ll - pos % 8};
            const auto bits{min(left, count)};
            out = out << bits |
                  (base[pos / 8] & 0xff >> (pos % 8)) >> (left - bits);
            count -= bits;
            skip(bits);
        }
        return out;
    }
    Int take_literal() {
        Int out{0ll};
        while (true) {
            const auto num{take(5)};
            out = out << 4 | num & 0b1111;
            if ((num & 0b10000) == 0) return out;
        }
    }
    Bits split(Int count) {
        skip(count);
        return {base, pos, pos - count};
    }
    bool is_empty() { return pos >= len; }
};

uint8_t to_digit(const char ch) {
    return isdigit(ch) ? static_cast<uint8_t>(ch - '0')
                       : static_cast<uint8_t>(ch - 'A' + 10);
}

Int solution1_helper(auto &&bits) {
    auto ver{bits.take(3)};
    if (bits.take(3) == 4) {
        bits.take_literal();
        return ver;
    }
    if (bits.take(1) == 0) {
        const auto len = bits.take(15);
        auto payload{bits.split(len)};
        while (!payload.is_empty()) ver += solution1_helper(payload);
    } else {
        for (const auto _ : iota(0, bits.take(11)))
            ver += solution1_helper(bits);
    }
    return ver;
}

vector<Int> solution2_helper(auto &&bits) {
    bits.skip(3);
    const auto typ{bits.take(3)};
    if (typ == 4) return {bits.take_literal()};
    vector<Int> nums{};
    nums.reserve(128);

    if (bits.take(1) == 0) {
        const auto len = bits.take(15);
        auto payload{bits.split(len)};
        while (!payload.is_empty()) {
            auto tmp{solution2_helper(payload)};
            nums.insert(nums.cend(), tmp.cbegin(), tmp.cend());
        }
    } else {
        for (const auto _ : iota(0, bits.take(11))) {
            auto tmp{solution2_helper(bits)};
            nums.insert(nums.cend(), tmp.cbegin(), tmp.cend());
        }
    }
    switch (typ) {
        case 0:
            return {accumulate(nums, 0ll)};
        case 1:
            return {accumulate(nums, 1ll, multiplies<Int>())};
        case 2:
            return {ranges::min(nums)};
        case 3:
            return {ranges::max(nums)};
        case 5:
            return {nums[0] > nums[1]};
        case 6:
            return {nums[0] < nums[1]};
        case 7:
            return {nums[0] == nums[1]};
        default:
            return {};
    }
}

Int solution1(const string_view str) {
    const auto res{str | chunk(2) | transform([](const auto &rng) {
                       return static_cast<uint8_t>(to_digit(rng.front()) << 4 |
                                                   to_digit(rng.back()));
                   }) |
                   to<vector>()};
    return solution1_helper(Bits{res});
}

Int solution2(const string_view str) {
    const auto res{str | chunk(2) | transform([](const auto &rng) {
                       return static_cast<uint8_t>(to_digit(rng.front()) << 4 |
                                                   to_digit(rng.back()));
                   }) |
                   to<vector>()};
    return solution2_helper(Bits{res}).front();
}

int main() {
    const string_view str{
#include "../../input/input16.txt"
    };
#ifdef BENCHMARK
    const auto point1{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution1(str);
    const auto point2{system_clock::now()};
    for (auto i{0}; i < 100; i++) solution2(str);
    const auto point3{system_clock::now()};
    cout << "\nDay16"
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
