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
    using namespace wasd314;
    auto dummy = [] {
        auto f = [](lint, int) { return std::vector<solver::solution_t<lint>>(); };
        return solver::named_solver<lint, decltype(f)>(f, "dummy");
    }();

    std::vector<solver::wrapped_solver<lint>> solvers{
        ee::re0_two_pointer<lint>,
        ee::re1_bs_all_1<lint>,
        ee::re1_bs_div_1<lint>,
        ee::re1_bs_all_pre<lint>,
        ee::re1_bs_div_pre<lint>,
    };
}  // namespace wasd314::e3


#endif  // CONSECUTIVE_POWER_SUM_E3
