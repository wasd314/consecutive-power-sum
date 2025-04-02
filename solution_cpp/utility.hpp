#ifndef CONSECUTIVE_POWER_SUM_UTILITY
#define CONSECUTIVE_POWER_SUM_UTILITY 1

#include <cassert>
#include <concepts>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <sstream>
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

    // `fs.size()` を N として，
    // - E == `E_FIRST` + 0 を `fs[0]` で
    // - E == `E_FIRST` + 1 を `fs[1]` で
    // - ...
    // - E == `E_FIRST` + N - 2 を `fs[N - 2]` で
    // - E >= `E_FIRST` + N - 1 を `fs[N - 1]` で
    //
    // それぞれ解いて結合する
    template<int E_FIRST>
    struct combined_solver {
        std::string name;
        std::vector<wrapped_solver> fs;

        combined_solver(std::initializer_list<wrapped_solver> args) : fs{args}
        {
            if (fs.empty()) return;
            name = fs[0].get().name;
            for (auto f : fs | std::views::drop(1)) {
                name += "-";
                name += f.get().name;
            }
        }
        combined_solver(std::string name, std::initializer_list<wrapped_solver> args) : name(name), fs{args}
        {
        }

        std::vector<solution_t> operator()(lint n) const
        {
            std::vector<solution_t> ans;

            int count = fs.size();
            for (int i = 0; i < count; ++i) {
                auto ansi = fs[i](n, E_FIRST + i);
                ans.insert(ans.end(), ansi.begin(), ansi.end());
            }
            for (int e = E_FIRST + count; e < 64; ++e) {
                auto ansi = fs[count - 1](n, e);
                ans.insert(ans.end(), ansi.begin(), ansi.end());
            }
            return ans;
        }
    };

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
            assert(("unsupportted e", false));
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

    template <int E_FIRST>
    void answer_power_with(const combined_solver<E_FIRST>& f)
    {
        lint n;
        std::cin >> n;
        std::vector<solution_t> ans = f(n);
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
        if (pred(lo)) return lo;
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

    lint saturating_pow(lint a, int e)
    {
        lint ans = 1;
        for (int i = 0; i < e; ++i) {
            if (__builtin_smulll_overflow(ans, a, &ans)) return std::numeric_limits<lint>::max();
        }
        return ans;
    }
}  // namespace wasd314

#endif  // CONSECUTIVE_POWER_SUM_UTILITY
