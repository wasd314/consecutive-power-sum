#ifndef CONSECUTIVE_POWER_SUM_E1
#define CONSECUTIVE_POWER_SUM_E1 1

#include "utility.hpp"

namespace wasd314::e1
{
    std::vector<solution_t> r13_pe_(lint n, int)
    {
        using namespace std;
        vector<solution_t> ans;
        auto pe_2n = wasd314::factorize(2 * n);
        auto ds = wasd314::list_divisors(pe_2n, [&](lint d) { return d <= (2 * n - 1) / d; });
        // auto ds = wasd314::list_divisors(pe_2n, [&](lint d) { return __int128_t(d) * d < 2 * n; });
        for (lint w : ds) {
            lint w2 = 2 * n / w;
            assert(w < w2);
            if (w % 2 == w2 % 2) continue;
            lint l = (w2 - w + 1) / 2;
            ans.emplace_back(1, l, l + w - 1);
        }
        ranges::sort(ans);
        return ans;
    }
    auto r13_pe = named_solver(r13_pe_, "r13_pe");
}  // namespace wasd314::e1

#endif  // CONSECUTIVE_POWER_SUM_E1
