#include <algorithm>
#include <iostream>
#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>


int main()
{
    using namespace std;
    using lint = long long;
    lint n;
    cin >> n;
    vector<tuple<int, lint, lint>> ans;

    auto saturating_pow = [](lint a, int e) {
        lint ans = 1;
        for (int i = 0; i < e; ++i) {
            if (__builtin_mul_overflow(ans, a, &ans)) return numeric_limits<lint>::max();
        }
        return ans;
    };
    auto solve_2 = [&](lint n, int e) {
        vector<lint> pows;
        // c = floor(sqrt(n)) + 1
        lint c = *ranges::lower_bound(views::iota(0ll, n + 2), true, {}, [&](lint x) { return saturating_pow(x, 2) > n; });
        // for (lint i = 0;; ++i) {
        //     lint ip = saturating_pow(i, e);
        //     if (ip > n) break;
        //     c++;
        // }
        cout << c << '\n';
        lint r = 1;
        lint current_sum = 0;
        for (lint l = 1; l < c; ++l) {
            while (r < c && current_sum + r * r <= n) {
                current_sum += r * r;
                r++;
            }
            if (current_sum == n) {
                ans.emplace_back(e, l, r - 1);
            }
            if (l < r) {
                current_sum -= l * l;
            }
        }
    };
    auto solve_two_pointer = [&](lint n, int e) {
        vector<lint> pows;
        for (lint i = 0;; ++i) {
            lint ip = saturating_pow(i, e);
            if (ip > n) break;
            pows.push_back(ip);
        }
        int c = pows.size();

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
    };
    solve_2(n, 2);
    // solve_two_pointer(n, 2);
    // for (int e = 3; e < 70; ++e) {
    //     solve_two_pointer(n, e);
    // }
    ranges::sort(ans);

    // cout << ans.size() << '\n';
    // for (auto [e, l, r] : ans) {
    //     cout << e << ' ' << l << ' ' << r << '\n';
    // }
}
