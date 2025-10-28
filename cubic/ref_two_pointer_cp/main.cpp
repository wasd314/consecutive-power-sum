#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <ranges>
#include <vector>

int main()
{
    using u64 = std::uint64_t;

    u64 n;
    std::cin >> n;
    assert(1 <= n && n <= 1'000'000'000'000'000'000);

    std::vector<std::pair<u64, u64>> ans;
    // 尺取り法
    // S(l, r-1) == s になるようにする
    u64 s = 0;
    for (u64 l = 1, r = 1; l * l * l <= n; ++l) {
        while (s + r * r * r <= n) {
            s += r * r * r;
            r++;
        }
        if (s == n) {
            ans.emplace_back(l, r - 1);
        }
        s -= l * l * l;
    }

    std::cout << ans.size() << '\n';
    for (auto [l, r] : ans) {
        std::cout << l << ' ' << r << '\n';
    }
}
