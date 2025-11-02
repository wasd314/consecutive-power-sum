#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <numeric>
#include <tuple>
#include <vector>

int main()
{
    using u64 = std::uint64_t;
    using solution_t = std::tuple<int, u64, u64>;
    u64 n;
    std::cin >> n;
    assert(2 <= n && n <= 1'000'000'000'000);

    // e = 1 の解を列挙する
    // 試し割り法により Θ(n^{1/2}) 時間
    auto solve_1 = [](u64 n) {
        std::vector<solution_t> ans;
        u64 n2 = n * 2;
        for (u64 w = 1; w * w < n2; ++w) {
            u64 wl = n2 / w;
            if (n2 % w || (w + wl) % 2 == 0) continue;
            u64 l = (wl - w + 1) / 2;
            ans.emplace_back(1, l, l + w - 1);
        }
        std::ranges::reverse(ans);
        return ans;
    };

    // min(a^e, u64 max) を返す
    auto saturating_pow = [](u64 a, int e) {
        u64 ans = 1;
        for (int i = 0; i < e; ++i) {
            if (__builtin_mul_overflow(ans, a, &ans)) return std::numeric_limits<u64>::max();
        }
        return ans;
    };

    // e = e の解を列挙する
    // 尺取り法を用いて Θ(n^{1/e}) 時間
    auto solve_e = [&saturating_pow](u64 n, int e) {
        std::vector<solution_t> ans;
        // S(e, l, r - 1) == s を保つようにする
        u64 s = 0;
        for (u64 l = 1, r = 1; saturating_pow(l, e) <= n; ++l) {
            while (s + saturating_pow(r, e) <= n) {
                s += saturating_pow(r, e);
                r++;
            }
            if (s == n) {
                ans.emplace_back(e, l, r - 1);
            }
            s -= saturating_pow(l, e);
        }
        return ans;
    };

    std::vector ans = solve_1(n);
    for (int e = 2; (n >> e) != 0; ++e) {
        std::vector ans_e = solve_e(n, e);
        ans.insert(ans.end(), ans_e.begin(), ans_e.end());
    }

    std::cout << ans.size() << '\n';
    for (auto [e, l, r] : ans) {
        std::cout << e << ' ' << l << ' ' << r << '\n';
    }
}
