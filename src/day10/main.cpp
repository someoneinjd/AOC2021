#include <array>
#include <iostream>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "../better_split.hpp"
#include "range/v3/algorithm/nth_element.hpp"
#include "range/v3/numeric/accumulate.hpp"
#include "range/v3/range/conversion.hpp"
#include "range/v3/view/reverse.hpp"
#include "range/v3/view/split.hpp"
#include "range/v3/view/transform.hpp"

#ifdef AOC_BENCH
#include <chrono>
using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::system_clock;
#endif

using ranges::accumulate;
using ranges::nth_element;
using ranges::to;
using ranges::views::reverse;
using ranges::views::split;
using ranges::views::transform;
using std::array;
using std::cout;
using std::erase;
using std::string_view;
using std::unordered_map;
using std::vector;

using Int = long long;

unordered_map<char, char> mappings{
    {'(', ')'}, {'<', '>'}, {'{', '}'}, {'[', ']'}};

Int solution1(const string_view str) {
    vector<char> stack(64);
    return accumulate(
        str | split('\n') | transform([&](const string_view pat) {
            stack.clear();
            for (const char ch : pat) {
                if (mappings.contains(ch))
                    stack.push_back(ch);
                else if (ch == mappings[stack.back()])
                    stack.pop_back();
                else
                    return ch;
            }
            return '\0';
        }) | transform([](const char ch) {
            return ch ? array{3ll, 25137ll, 57ll, 1197ll}[ch / 30 - 1] : 0ll;
        }),
        0ll);
}

Int solution2(const string_view str) {
    vector<char> stack(64);
    auto scores{str | split('\n') | transform([&](const string_view pat) {
                    stack.clear();
                    for (const char ch : pat) {
                        if (mappings.contains(ch))
                            stack.push_back(ch);
                        else if (ch == mappings[stack.back()])
                            stack.pop_back();
                        else
                            return 0ll;
                    }
                    return accumulate(
                        stack | reverse | transform([](const char ch) {
                            return mappings[ch];
                        }),
                        0ll, [](const Int acc, const Int ch) {
                            return acc * 5 + array{1, 4, 2, 3}[ch / 30 - 1];
                        });
                }) |
                to<vector<Int>>()};
    erase(scores, 0ll);
    nth_element(scores, scores.begin() + (scores.size() / 2));
    return scores[scores.size() / 2];
}

int main() {
    const string_view str{
#include "../../input/input10.txt"
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
