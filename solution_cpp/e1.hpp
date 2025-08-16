#ifndef CONSECUTIVE_POWER_SUM_E1
#define CONSECUTIVE_POWER_SUM_E1 1

#include "utility.hpp"

namespace wasd314::e1
{
    using namespace wasd314::solver;
    std::vector<solution_t<u128>> r13_pe_(u128 n, int)
    {
        using namespace std;
        using namespace factorization;
        vector<solution_t<u128>> ans;
        auto pe_2n = to_pairs(factorize(2 * n));
        auto ds = list_divisors(pe_2n, [&](u128 d) { return d <= (2 * n - 1) / d; });
        // auto ds = wasd314::list_divisors(pe_2n, [&](lint d) { return __int128_t(d) * d < 2 * n; });
        for (u128 w : ds) {
            u128 w2 = 2 * n / w;
            assert(w < w2);
            if (w % 2 == w2 % 2) continue;
            u128 l = (w2 - w + 1) / 2;
            ans.emplace_back(1, l, l + w - 1);
        }
        ranges::sort(ans);
        return ans;
    }
    auto r13_pe = named_solver<u128, decltype(&r13_pe_)>(r13_pe_, "r13_pe");
}  // namespace wasd314::e1

#endif  // CONSECUTIVE_POWER_SUM_E1
