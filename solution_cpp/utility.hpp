#ifndef CONSECUTIVE_POWER_SUM_UTILITY
#define CONSECUTIVE_POWER_SUM_UTILITY 1

#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <ranges>
#include <sstream>
#include <tuple>
#include <vector>

namespace wasd314
{
    using lint = long long;

    template <typename T>
    using solution_t = std::tuple<int, T, T>;

    template <typename T>
    struct named_solver_base {
        std::string name;
        virtual std::vector<solution_t<T>> operator()(T n, int e) const
        {
            std::cerr << "virtual function called(" << n << "," << e << ")" << std::endl;
            return {};
        }
        const named_solver_base& get() const { return *this; }
    };
    template <typename T, typename F>
    struct named_solver : named_solver_base {
        F f;
        named_solver(F f, std::string name) : f(f) { this->name = name; }
        std::vector<solution_t<T>> operator()(T n, int e) const override { return f(n, e); }
    };
    template <typename T>
    using wrapped_solver = std::reference_wrapper<named_solver_base<T>>;
    // 本体の関数 f の型 F の差異を named_solver_base で吸収し，
    // std::reference_wrapper に包むことで f を実行し分けている

    // `fs.size()` を N として，
    // - E == `E_FIRST` + 0 を `fs[0]` で
    // - E == `E_FIRST` + 1 を `fs[1]` で
    // - ...
    // - E == `E_FIRST` + N - 2 を `fs[N - 2]` で
    // - E >= `E_FIRST` + N - 1 を `fs[N - 1]` で
    //
    // それぞれ解いて結合する
    template <typename T, int E_FIRST>
    struct combined_solver {
        std::string name;
        std::vector<wrapped_solver<T>> fs;

        combined_solver(std::initializer_list<wrapped_solver<T>> args) : fs{args}
        {
            if (fs.empty()) return;
            name = fs[0].get().name;
            for (auto f : fs | std::views::drop(1)) {
                name += "-";
                name += f.get().name;
            }
        }
        combined_solver(std::string name, std::initializer_list<wrapped_solver<T>> args) : name(name), fs{args}
        {
        }

        std::vector<solution_t<T>> operator()(T n) const
        {
            std::vector<solution_t<T>> ans;

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

    template <typename T>
    std::vector<T> read_all()
    {
        T n;
        std::vector<T> ans;
        while (std::cin >> n) {
            ans.push_back(n);
        }
        if (ans.size() > 1) {
            assert(ans.size() == ans[0] + 1);
            ans.erase(ans.begin());
        }
        return ans;
    }

    void answer_cubic_with(const wrapped_solver<lint>& f)
    {
        using std::cout;
        auto ns = read_all<lint>();
        for (lint n : ns) {
            auto ans = f(n, 3);
            cout << ans.size() << '\n';
            for (auto [e, l, r] : ans) {
                cout << l << ' ' << r << '\n';
            }
        }
    }

    template <typename T, int E_FIRST>
    void answer_power_with(const combined_solver<T, E_FIRST>& f)
    {
        using std::cout;
        auto ns = read_all();
        for (T n : ns) {
            auto ans = f(n);
            cout << ans.size() << '\n';
            for (auto [e, l, r] : ans) {
                cout << e << ' ' << l << ' ' << r << '\n';
            }
        }
    }

    // min i in [lo, hi) s.t. pred(i), or default = hi
    template <typename T, typename F>
    requires requires(F pred, T l) {
        { pred(l) } -> std::same_as<bool>;
    }
    T bisect_left(T lo, T hi, F pred)
    {
        if (pred(lo)) return lo;
        while (lo + 1 < hi) {
            T mi = lo + (hi - lo) / 2;
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

    bool is_prime(lint n)
    {
        if (n < 2) return false;
        for (lint p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
            if (n == p) return true;
            if (n % p == 0) return false;
        }
        if (n < 41 * 41) return true;
        auto mul_mod = [&](lint x, lint y) -> lint {
            return __int128_t(x) * y % n;
        };
        auto pow_mod = [&](lint x, lint e) {
            lint p = 1;
            while (e) {
                if (e & 1) p = mul_mod(p, x);
                x = mul_mod(x, x);
                e >>= 1;
            }
            return p;
        };
        auto test_miller_rabin = [&](const std::vector<lint>& bases) {
            int e = std::countr_zero((unsigned long long)(n - 1));
            lint o = n >> e;
            for (lint b : bases) {
                lint x = pow_mod(b, o);
                if (x == 1) continue;
                for (int ei = 0; ei < e; ++ei) {
                    lint y = mul_mod(x, x);
                    if (y == 1) {
                        if (x == n - 1) break;
                        return false;
                    }
                    x = y;
                    if (ei == e - 1) return false;
                }
            }
            return true;
        };
        if (n < 2047) return test_miller_rabin({2});
        if (n < 9080191) return test_miller_rabin({31, 73});
        if (n < 4759123141) return test_miller_rabin({2, 7, 61});
        if (n < 1122004669633) return test_miller_rabin({2, 13, 23, 1662803});
        if (n < 3770579582154547) return test_miller_rabin({2, 880937, 2570940, 610386380, 4130785767});
        return test_miller_rabin({2, 325, 9375, 28178, 450775, 9780504, 1795265022});
    }

    std::vector<std::pair<lint, int>> factorize(lint n)
    {
        using namespace std;
        auto mul_mod = [&](lint x, lint y) -> lint {
            return __int128_t(x) * y % n;
        };
        if (n == 1) return {};

        random_device seed;
        mt19937_64 gen(seed());
        uniform_int_distribution<lint> dist(1, n - 1);
        auto find_prime_factor = [&](auto self, lint nn) -> lint {
            if (is_prime(nn)) return nn;
            if (nn % 2 == 0) return 2;
            lint c;
            auto f = [&](lint x) { return (mul_mod(x, x) + c) % nn; };
            while (true) {
                c = dist(gen);
                lint x = dist(gen), y = x;
                lint d = 1;
                while (d == 1) {
                    x = f(x);
                    y = f(f(y));
                    d = gcd(abs(x - y), nn);
                }
                if (d == nn) continue;
                if (is_prime(d)) return d;
                if (is_prime(nn / d)) return nn / d;
                return self(self, min(d, nn / d));
            }
        };
        vector<pair<lint, int>> ans;
        while (n > 1) {
            lint p = find_prime_factor(find_prime_factor, n);
            int e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            ans.emplace_back(p, e);
        }
        ranges::sort(ans);
        return ans;
    }

    // [d for d divides n if pred(d)]
    // pred(d): small enough
    template <typename T>
    requires requires(T pred, lint l) {
        { pred(l) } -> std::same_as<bool>;
    }
    std::vector<lint> list_divisors(const std::vector<std::pair<lint, int>>& pe, T pred)
    {
        if (!pred(1)) return {};
        std::vector<lint> ans{1};
        for (auto [p, e] : pe) {
            for (int i = 0, s = ans.size(); i < s; ++i) {
                lint d = ans[i];
                for (int ei = 1; ei <= e; ++ei) {
                    d *= p;
                    if (!pred(d)) break;
                    ans.push_back(d);
                }
            }
        }
        return ans;
    }

    std::vector<int> parse_range(int size, const std::string& line)
    {
        std::vector<int> todo;
        for (auto sv : std::ranges::split_view(line, " ")) {
            std::string arg(sv.begin(), sv.end());
            auto pos = arg.find("-");
            if (arg == "-") {
                for (int e = 0; e < size; ++e) {
                    todo.push_back(e);
                }
            } else if (pos != std::string::npos) {
                int l = std::stoi(arg.substr(0, pos));
                int r = std::stoi(arg.substr(pos + 1));
                l = std::max(l, 0);
                r = std::min(r + 1, size);
                for (int e = l; e < r; ++e) {
                    todo.push_back(e);
                }
            } else {
                int e = std::stoi(arg);
                if (0 <= e && e < size) {
                    todo.push_back(e);
                }
            }
        }
        std::ranges::sort(todo);
        auto dup = std::ranges::unique(todo);
        todo.erase(dup.begin(), dup.end());
        return todo;
    }
}  // namespace wasd314

#endif  // CONSECUTIVE_POWER_SUM_UTILITY
