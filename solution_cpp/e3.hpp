#ifndef CONSECUTIVE_POWER_SUM_E3
#define CONSECUTIVE_POWER_SUM_E3 1

#include <algorithm>
#include <concepts>
#include <functional>
#include <iostream>
#include <ranges>
#include <tuple>
#include <vector>

#include "ee.hpp"
#include "utility.hpp"

namespace wasd314::e3
{
    auto dummy = named_solver([](lint, int) { return std::vector<solution_t>(); }, "dummy");

    std::vector<wrapped_solver> solvers{
        ee::re0_two_pointer,
        ee::re1_bs_all_1,
        ee::re1_bs_div_1,
        ee::re1_bs_all_pre,
        ee::re1_bs_div_pre,
    };
}  // namespace wasd314::e3


#endif  // CONSECUTIVE_POWER_SUM_E3
