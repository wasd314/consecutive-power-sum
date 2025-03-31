#ifndef CONSECUTIVE_POWER_SUM_UTILITY
#define CONSECUTIVE_POWER_SUM_UTILITY 1

#include <concepts>
#include <iostream>
#include <numeric>
#include <ranges>
#include <tuple>
#include <vector>

namespace wasd314
{
    using lint = long long;
    using solution_t = std::tuple<int, lint, lint>;

    struct named_solver_base {
        std::string name;
        virtual std::vector<solution_t> operator()(lint n, int e) const
        {
            std::cerr << "virtual function called(" << n << "," << e << ")" << std::endl;
            return {};
        }
        const named_solver_base& get() const { return *this; }
    };
    template <typename T>
    struct named_solver : named_solver_base {
        T f;
        named_solver(T f, std::string name) : f(f) { this->name = name; }
        std::vector<solution_t> operator()(lint n, int e) const override { return f(n, e); }
    };
    using wrapped_solver = std::reference_wrapper<named_solver_base>;
    // 本体の関数 f の型 T の差異を named_solver_base で吸収し，
    // std::reference_wrapper に包むことで f を実行し分けている

    // e-th power sum of [l, r) or [0, l)
    __int128_t power_sum(int e, __int128_t l, __int128_t r = -10)
    {
        if (r == -10) {
            r = l;
            l = 0;
        }
        __int128_t w = r - l;
        if (e == 1) {
            return l * w + w * (w - 1) / 2;
        } else if (e == 2) {
            return l * w * (l + w - 1) + w * (w - 1) / 2 * (2 * w - 1) / 3;
        } else if (e == 3) {
            return w * (2 * l + w - 1) / 2 * l * (l + w - 1) + w * w * (w - 1) / 2 * (2 * l + w - 1) / 2;
        } else if (e == 4) {
            return l * w * (l + w - 1) * (l * (l + w - 1) + w * (w - 1)) + w * (w - 1) / 2 * (2 * w - 1) / 3 * (3 * w * (w - 1) - 1) / 5;
        } else if (e == 5) {
            return w * (2 * l + w - 1) / 2 * (l * l * l * l + (2 * l + w) * (w - 1) / 2 * (2 * l * (3 * l + 2 * w - 1) + 2 * w * (w - 1) - 1) / 3);
        } else {
            return 0;
        }
    }

    void answer_cubic_with(const wrapped_solver& f)
    {
        using std::cout;
        using std::endl;

        lint n;
        std::cin >> n;
        std::vector<solution_t> ans = f(n, 3);
        cout << ans.size() << endl;
        for (auto [e, l, r] : ans) {
            cout << l << ' ' << r << endl;
        }
    }

    // `sols.size()` を N として，
    // - E == `E_FIRST` + 0 を `sols[0]` で
    // - E == `E_FIRST` + 1 を `sols[1]` で
    // - ...
    // - E == `E_FIRST` + N - 2 を `sols[N - 2]` で
    // - E >= `E_FIRST` + N - 1 を `sols[N - 1]` で
    //
    // それぞれ解いて結合する
    template <int E_FIRST>
    requires(E_FIRST == 1 || E_FIRST == 2)
    void answer_power_with(const std::vector<wrapped_solver>& sols)
    {
        lint n;
        std::cin >> n;
        std::vector<solution_t> ans;

        int count = sols.size();
        for (int i = 0; i < count - 1; ++i) {
            auto ansi = sols[i](n, E_FIRST + i);
            ans.insert(ans.end(), ansi.begin(), ansi.end());
        }
        for (int e = E_FIRST + count - 1; e < 64; ++e) {
            auto ansi = sols[count - 1](n, e);
            ans.insert(ans.end(), ansi.begin(), ansi.end());
        }
        std::cout << ans.size() << std::endl;
        for (auto [e, l, r] : ans) {
            std::cout << e << ' ' << l << ' ' << r << std::endl;
        }
    }

    template <typename T>
    requires requires(T pred, lint l) {
        { pred(l) } -> std::same_as<bool>;
    }
    lint bisect_left(lint lo, lint hi, T pred)
    {
        while (lo + 1 < hi) {
            lint mi = std::midpoint(lo, hi);
            if (pred(mi)) {
                hi = mi;
            } else {
                lo = mi;
            }
        }
        return hi;
    }
}  // namespace wasd314

#endif  // CONSECUTIVE_POWER_SUM_UTILITY
