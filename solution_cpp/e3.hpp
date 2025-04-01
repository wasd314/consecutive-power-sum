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
#include "ee.hpp"

namespace wasd314::e3
{
    auto dummy = named_solver([](lint, int) { return std::vector<solution_t>(); }, "dummy");

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

    std::vector<wrapped_solver> solvers{
        r30_two_pointer,
        ee::re1_bs_all_1,
        ee::re1_bs_div_1,
        ee::re1_bs_all_pre,
        ee::re1_bs_div_pre,
    };
}  // namespace wasd314::e3


#endif  // CONSECUTIVE_POWER_SUM_E3
