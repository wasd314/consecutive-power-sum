#ifndef CONSECUTIVE_POWER_SUM_E2
#define CONSECUTIVE_POWER_SUM_E2 1

#include "ee.hpp"

namespace wasd314::e2 {
    std::vector<combined_solver<2>> solvers_ac{
        {{ee::re1[0], ee::re0[0]}},
        {{ee::re1[1], ee::re0[0]}},
        {{ee::re1[2], ee::re0[0]}},
        {{ee::re1[3], ee::re0[0]}},
    };
    std::vector<combined_solver<2>> solvers_tle{
        {{ee::re0[0], ee::re0[0]}},
    };
}

#endif  // CONSECUTIVE_POWER_SUM_E2
