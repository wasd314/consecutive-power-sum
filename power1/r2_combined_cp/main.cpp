#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
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
    using T = tuple<int, lint, lint>;
    auto solve_1 = [](lint n) {
        vector<T> ans;
        lint n2 = n * 2;
        for (lint w = 1; w * w <= n2; ++w) {
            if (n2 % w) continue;
            lint wl = n2 / w;
            if (w >= wl || (w + wl) % 2 == 0) continue;
            lint l = (wl - w + 1) / 2;
            ans.emplace_back(1, l, l + w - 1);
        }
        ranges::reverse(ans);
        return ans;
    };
    auto solve_2 = [](lint n) {
        lint m = 1;
        for (int i = 30; i-- > 0;) {
            lint m2 = m + (1ll << i);
            if (m2 * m2 <= n) m = m2;
        }
        assert(m * m <= n);
        assert(n < (m + 1) * (m + 1));
        vector<T> ans;
        lint r = 1, s = 0;
        for (lint l = 1; l <= m; ++l) {
            while (s + r * r <= n) {
                s += r * r;
                r++;
            }
            if (s == n) ans.emplace_back(2, l, r - 1);
            s -= l * l;
        }
        return ans;
    };
    auto saturating_pow = [](lint a, int e) {
        lint ans = 1;
        for (int i = 0; i < e; ++i) {
            if (__builtin_mul_overflow(ans, a, &ans)) return std::numeric_limits<lint>::max();
        }
        return ans;
    };
    auto solve_e = [&saturating_pow](lint n, int e) {
        vector<T> ans;
        vector<lint> acc{0};
        for (lint i = 1; saturating_pow(i, e) <= n; ++i) {
            acc.push_back(acc.back() + saturating_pow(i, e));
        }
        map<lint, lint> to_l;
        for (auto [l, s] : acc | views::enumerate) to_l[s] = l;
        for (auto [r, s] : acc | views::enumerate) {
            if (r == 0) continue;
            auto it = to_l.find(s - n);
            if (it == to_l.end()) continue;
            lint l = it->second + 1;
            if (l >= 1) ans.emplace_back(e, l, r);
        }
        return ans;
    };
    vector<T> ans = solve_1(n);
    for (auto t : solve_2(n)) ans.push_back(t);
    for (int e = 3; saturating_pow(2, e) <= n; ++e) {
        for (auto t : solve_e(n, e)) ans.push_back(t);
    }
    cout << ans.size() << '\n';
    for (auto [e, l, r] : ans) {
        cout << e << ' ' << l << ' ' << r << '\n';
    }
}
