#ifndef CONSECUTIVE_POWER_SUM_EE
#define CONSECUTIVE_POWER_SUM_EE 1

#include <vector>

#include "utility.hpp"

namespace wasd314::ee
{
    std::vector<solution_t> re0_two_pointer_(lint n, int e)
    {
        using std::vector;
        vector<lint> pows;
        for (lint i = 0;; ++i) {
            lint ip = saturating_pow(i, e);
            if (ip > n) break;
            pows.push_back(ip);
        }
        int c = pows.size();
        vector<solution_t> ans;

        int r = 1;
        lint current_sum = 0;
        for (int l = 1; l < c; ++l) {
            while (r < c && current_sum + pows[r] <= n) {
                current_sum += pows[r];
                r++;
            }
            if (current_sum == n) {
                ans.emplace_back(e, l, r - 1);
            }
            if (l < r) {
                current_sum -= pows[l];
            }
        }
        std::ranges::sort(ans);
        return ans;
    }
    auto re0_two_pointer = named_solver(re0_two_pointer_, "re0_two_pointer");


    // min D s.t. W divides [D * S(e, L, L+W-1)] for all L & W.
    const int enough_denom[] = {1, 2, 6, 2, 30, 2, 42, 2, 30, 2, 66, 2, 2730, 2, 6, 2, 510, 2, 798, 2, 330, 2, 138, 2, 2730, 2, 6, 2, 870, 2, 14322, 2, 510, 2, 6, 2, 1919190, 2, 6, 2, 13530, 2, 1806, 2, 690, 2, 282, 2, 46410, 2, 66, 2, 1590, 2, 798, 2, 870, 2, 354, 2, 56786730, 2, 6, 2, 510, 2, 64722, 2, 30, 2, 4686, 2, 140100870, 2, 6, 2, 30, 2, 3318, 2, 230010, 2, 498, 2, 3404310, 2, 6, 2, 61410, 2, 272118, 2, 1410, 2, 6, 2, 4501770, 2, 6, 2};

    template <bool only_div, bool from_prev>
    auto re1_bs_()
    {
        using std::literals::operator""s;

        auto f = [](lint n, int e) {
            using std::vector;
            vector<solution_t> ans;
            lint prev_l = n;
            for (lint w = 1;; ++w) {
                if (power_sum(e, 1, w + 1) > n) break;
                if (only_div && (enough_denom[e] * n % w) != 0) continue;
                lint l;
                auto pred = [&](lint li) { return power_sum(e, li, li + w) >= n; };
                if (from_prev && w != 1) {
                    lint dl = 1;
                    while (dl < prev_l && pred(prev_l - dl)) {
                        dl <<= 1;
                    }
                    l = bisect_left(std::max(0ll, prev_l - dl), prev_l, pred);
                } else {
                    lint r = 1;
                    while (!pred(r)) {
                        r <<= 1;
                    }
                    l = bisect_left(0, r, pred);
                }
                prev_l = l;
                if (power_sum(e, l, l + w) == n) {
                    ans.emplace_back(e, l, l + w - 1);
                }
            }
            std::ranges::reverse(ans);
            // std::ranges::sort(ans);
            return ans;
        };
        auto name = "re1_bs"s + (only_div ? "_div"s : "_all"s) + (from_prev ? "_pre"s : "_1"s);
        return named_solver(f, name);
    }
    auto re1_bs_all_1 = re1_bs_<false, false>();
    auto re1_bs_div_1 = re1_bs_<true, false>();
    auto re1_bs_all_pre = re1_bs_<false, true>();
    auto re1_bs_div_pre = re1_bs_<true, true>();

    std::vector<wrapped_solver> re1{
        re1_bs_all_1,
        re1_bs_div_1,
        re1_bs_all_pre,
        re1_bs_div_pre,
    };
}  // namespace wasd314::ee

#endif  // CONSECUTIVE_POWER_SUM_EE
