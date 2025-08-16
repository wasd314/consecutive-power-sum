#ifndef CONSECUTIVE_POWER_SUM_E2
#define CONSECUTIVE_POWER_SUM_E2 1

#include "ee.hpp"

namespace wasd314::e2
{
    using namespace wasd314;

    std::vector<solver::combined_solver<lint, 2>> solvers_ac{
        {{ee::re1<lint>[0], ee::re0<lint>[0]}},
        {{ee::re1<lint>[1], ee::re0<lint>[0]}},
        {{ee::re1<lint>[2], ee::re0<lint>[0]}},
        {{ee::re1<lint>[3], ee::re0<lint>[0]}},
    };
    std::vector<solver::combined_solver<lint, 2>> solvers_tle{
        {{ee::re0<lint>[0], ee::re0<lint>[0]}},
    };
}  // namespace wasd314::e2

#endif  // CONSECUTIVE_POWER_SUM_E2
