#ifndef CONSECUTIVE_POWER_SUM_E3
#define CONSECUTIVE_POWER_SUM_E3 1

#include <algorithm>
#include <concepts>
#include <functional>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>

#include "utility.hpp"

namespace wasd314::e3
{
    std::vector<solution_t> r30_two_pointer_(lint n, int)
    {
        using std::vector;
        vector<lint> cube;
        for (lint i = 0;; ++i) {
            lint i3 = i * i * i;
            if (i3 > n) break;
            cube.push_back(i3);
        }
        int c = cube.size();
        vector<solution_t> ans;

        int r = 1;
        lint current_sum = 0;
        for (int l = 1; l < c; ++l) {
            while (r < c && current_sum + cube[r] <= n) {
                current_sum += cube[r];
                r++;
            }
            if (current_sum == n) {
                ans.emplace_back(3, l, r - 1);
            }
            if (l < r) {
                current_sum -= cube[l];
            }
        }
        std::ranges::sort(ans);
        return ans;
    }
    auto r30_two_pointer = named_solver(r30_two_pointer_, "r30_two_pointer");

    template <bool only_div, bool from_prev>
    auto r31_bs()
    {
        using namespace std::literals::string_literals;

        auto f = [](lint n, int) {
            using std::vector;
            vector<solution_t> ans;
            lint prev_l = n;
            for (lint w = 1;; ++w) {
                if (power_sum(3, 1, w + 1) > n) break;
                if (only_div && (4 * n % w) != 0) continue;
                lint l;
                if (from_prev && w != 1) {
                    lint dl = 1;
                    while (dl < prev_l && power_sum(3, prev_l - dl, prev_l - dl + w) > n) {
                        dl <<= 1;
                    }
                    l = bisect_left(std::max(0ll, prev_l - dl), prev_l + 1, [&](lint li) { return power_sum(3, li, li + w) >= n; });
                } else {
                    lint r = 1;
                    while (power_sum(3, r, r + w) <= n) {
                        r <<= 1;
                    }
                    l = bisect_left(0, r + 1, [&](lint li) { return power_sum(3, li, li + w) >= n; });
                }
                prev_l = l;
                if (power_sum(3, l, l + w) == n) {
                    ans.emplace_back(3, l, l + w - 1);
                }
            }
            std::ranges::reverse(ans);
            // std::ranges::sort(ans);
            return ans;
        };
        auto name = "r31_bs_"s + (only_div ? "div"s : "all"s) + (from_prev ? "pre"s : "1"s);
        return named_solver(f, name);
    }
    auto r31_bs_all_1 = r31_bs<false, false>();
    auto r31_bs_div_1 = r31_bs<true, false>();
    auto r31_bs_all_pre = r31_bs<false, true>();
    auto r31_bs_div_pre = r31_bs<true, true>();

    std::vector<wrapped_solver> solvers{
        r30_two_pointer,
        r31_bs_all_1,
        r31_bs_div_1,
        r31_bs_all_pre,
        r31_bs_div_pre,
    };
}  // namespace wasd314::e3


#endif  // CONSECUTIVE_POWER_SUM_E3
