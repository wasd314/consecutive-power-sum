#ifndef CONSECUTIVE_POWER_SUM_EE
#define CONSECUTIVE_POWER_SUM_EE 1

#include <vector>

#include "utility.hpp"

namespace wasd314::ee
{
    using namespace wasd314::solver;
    template <typename T>
    std::vector<solution_t<T>> re0_two_pointer_(T n, int e)
    {
        using std::vector;
        vector<T> pows;
        for (T i = 0;; ++i) {
            T ip = saturating_pow(i, e);
            if (ip > n) break;
            pows.push_back(ip);
        }
        int c = pows.size();
        vector<solution_t<T>> ans;

        int r = 1;
        T current_sum = 0;
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
    template <typename T>
    auto re0_two_pointer = named_solver<T, decltype(&re0_two_pointer_<T>)>(re0_two_pointer_<T>, "re0_two_pointer");

    // min D s.t. W divides [D * S(e, L, L+W-1)] for all L & W.
    const int enough_denom[] = {1, 2, 6, 2, 30, 2, 42, 2, 30, 2, 66, 2, 2730, 2, 6, 2, 510, 2, 798, 2, 330, 2, 138, 2, 2730, 2, 6, 2, 870, 2, 14322, 2, 510, 2, 6, 2, 1919190, 2, 6, 2, 13530, 2, 1806, 2, 690, 2, 282, 2, 46410, 2, 66, 2, 1590, 2, 798, 2, 870, 2, 354, 2, 56786730, 2, 6, 2, 510, 2, 64722, 2, 30, 2, 4686, 2, 140100870, 2, 6, 2, 30, 2, 3318, 2, 230010, 2, 498, 2, 3404310, 2, 6, 2, 61410, 2, 272118, 2, 1410, 2, 6, 2, 4501770, 2, 6, 2};

    template <typename T, bool ONLY_DIV, bool FROM_PREV>
    auto re1_bs_()
    {
        using std::literals::operator""s;

        auto f = [](T n, int e) {
            std::vector<solution_t<T>> ans;
            T prev_l = n;
            for (T w = 1;; ++w) {
                if (power_sum(e, 1, w + 1) > n) break;
                if (ONLY_DIV && (enough_denom[e] * n % w) != 0) continue;
                T l;
                auto pred = [&](T li) { return power_sum(e, li, li + w) >= n; };
                if (FROM_PREV && w != 1) {
                    T dl = 1;
                    while (dl < prev_l && pred(prev_l - dl)) {
                        dl <<= 1;
                    }
                    l = bisect_left(prev_l >= dl ? prev_l - dl : T{0}, prev_l, pred);
                } else {
                    T r = 1;
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
        auto name = "re1_bs"s + (ONLY_DIV ? "_div"s : "_all"s) + (FROM_PREV ? "_pre"s : "_1"s);
        return named_solver<T, decltype(f)>(f, name);
    }
    template <typename T>
    auto re1_bs_all_1 = re1_bs_<T, false, false>();
    template <typename T>
    auto re1_bs_div_1 = re1_bs_<T, true, false>();
    template <typename T>
    auto re1_bs_all_pre = re1_bs_<T, false, true>();
    template <typename T>
    auto re1_bs_div_pre = re1_bs_<T, true, true>();

    template <typename T>
    std::vector<wrapped_solver<T>> re0{
        re0_two_pointer<T>,
    };
    template <typename T>
    std::vector<wrapped_solver<T>> re1{
        re1_bs_all_1<T>,
        re1_bs_div_1<T>,
        re1_bs_all_pre<T>,
        re1_bs_div_pre<T>,
    };
}  // namespace wasd314::ee

#endif  // CONSECUTIVE_POWER_SUM_EE
