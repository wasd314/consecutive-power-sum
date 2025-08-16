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
    using i16 = std::int16_t;
    using u16 = std::uint16_t;
    using i32 = std::int32_t;
    using u32 = std::uint32_t;
    using i64 = std::int64_t;
    using u64 = std::uint64_t;
    using i128 = __int128_t;
    using u128 = __uint128_t;

    namespace io
    {
        constexpr u128 parse_u128(const std::string &s)
        {
            u128 n = 0;
            for (char c : s) {
                if ('0' <= c && c <= '9') {
                    n = n * 10 + (c - '0');
                }
            }
            return n;
        }
        std::istream &operator>>(std::istream &is, u128 &n)
        {
            std::istream::sentry sen(is);
            if (sen) {
                std::string s;
                is >> s;
                n = parse_u128(s);
            }
            return is;
        }
        std::ostream &operator<<(std::ostream &os, u128 n)
        {
            std::ostream::sentry sen(os);
            if (sen) {
                if (n == 0) os << '0';
                std::string ns;
                while (n) {
                    int r = n % 10;
                    n /= 10;
                    ns += char('0' + r);
                }
                std::ranges::reverse(ns);
                os << ns;
            }
            return os;
        }
    }  // namespace io

    inline namespace literals
    {
        constexpr u128 operator""_u128(const char *s) { return io::parse_u128(s); }
    }  // namespace literals

    namespace factorization
    {
        template <int MOD_BITS>
        struct integer_pack;
        template <>
        struct integer_pack<32> {
            using I0 = i16;
            using U0 = u16;
            using I1 = i32;
            using U1 = u32;
            using I2 = i64;
            using U2 = u64;
            static constexpr int BITS_0 = std::numeric_limits<U0>::digits;
            static constexpr int BITS_1 = std::numeric_limits<U1>::digits;
            static constexpr int BITS_2 = std::numeric_limits<U2>::digits;
        };
        template <>
        struct integer_pack<64> {
            using I0 = i64;
            using U0 = u64;
            using I1 = i64;
            using U1 = u64;
            using I2 = i128;
            using U2 = u128;
            static constexpr int BITS_0 = std::numeric_limits<U0>::digits;
            static constexpr int BITS_1 = std::numeric_limits<U1>::digits;
            static constexpr int BITS_2 = std::numeric_limits<U2>::digits;
        };
        template <>
        struct integer_pack<128> {
            using I0 = i64;
            using U0 = u64;
            using I1 = i128;
            using U1 = u128;
            static constexpr int BITS_0 = std::numeric_limits<U0>::digits;
            static constexpr int BITS_1 = std::numeric_limits<U1>::digits;
        };

        template <int MOD_BITS>
        struct dynamic_mod {
            using pack = integer_pack<MOD_BITS>;
            using U0 = pack::U0;
            using U1 = pack::U1;
            using I1 = pack::I1;
            static constexpr int BITS_0 = pack::BITS_0;
            static constexpr int BITS_1 = pack::BITS_1;

            // N
            U1 mod;
            // R^-1 % N
            U1 R_1;
            // R^1 % N
            U1 R1;
            // R^2 % N
            U1 R2;
            // -(N^-1) % R
            U1 N_;

            // `(x1 * x2) >> BITS_1`
            static U1 multiply_high(U1 x, U1 y)
            {
                U0 hx = x >> BITS_0, lx = x;
                U0 hy = y >> BITS_0, ly = y;
                U1 ans = U1(hx) * hy;
                ans += (U1(hx) * ly) >> BITS_0;
                ans += (U1(lx) * hy) >> BITS_0;
                U1 m = U1(hx * ly) + U1(lx * hy) + ((U1(lx) * ly) >> BITS_0);
                ans += m >> BITS_0;
                return ans;
            }

           private:
            void set_N_()
            {
                U1 n_inv = mod;
                for (int bit = 2; bit < BITS_1; bit <<= 1) {
                    n_inv *= 2 - n_inv * mod;
                }
                N_ = -n_inv;
            }
            void set_R1() { R1 = -mod % mod; }
            void set_R2()
            {
                R2 = R1;
                for (int _ = 0; _ < BITS_1; ++_) {
                    R2 <<= 1;
                    if (R2 >= mod) R2 -= mod;
                }
            }
            void set_R_1() { R_1 = 1 + multiply_high(mod, N_); }

           public:
            dynamic_mod(const U1 &modulo) : mod(modulo)
            {
                assert(~mod >> (BITS_1 - 1));
                assert(mod & 1);
                set_N_();
                set_R1();
                set_R2();
                set_R_1();
            }

            U1 safe_mod(I1 x) const
            {
                x %= I1(mod);
                if (x < 0) x += I1(mod);
                return x;
            }
            // MR(x)
            U1 reduce(const U1 &x) const { return multiply_reduce(x, 1); }
            // MR(x * y)
            U1 multiply_reduce(const U1 &x, const U1 &y) const
            {
                U1 t_ = x * y * N_;
                U1 t = multiply_high(x, y) + multiply_high(t_, mod) + (x * y != 0);
                return t < mod ? t : t - mod;
            }

            U1 from(const U1 &x) const { return multiply_reduce(x % mod, R2); }
            U1 from(U1 &&x) const { return from(x); }
            U1 from_i(const I1 &x) const { return multiply_reduce(safe_mod(x), R2); }
            U1 from_i(I1 &&x) const { return from_i(x); }

            U1 val(const U1 &rx) const { return reduce(rx); }
            U1 pow(U1 rx, U1 e) const
            {
                U1 ans = R1, b = rx;
                while (e) {
                    if (e & 1) ans = multiply_reduce(ans, b);
                    b = multiply_reduce(b, b);
                    e >>= 1;
                }
                return ans;
            }

            U1 add(const U1 &x, const U1 &y) const
            {
                U1 z = x + y;
                if (z >= mod) z -= mod;
                return z;
            }
            U1 sub(const U1 &x, const U1 &y) const
            {
                U1 z;
                if (__builtin_sub_overflow(x, y, &z)) z += mod;
                return z;
            }
            U1 mul(const U1 &x, const U1 &y) const { return multiply_reduce(x, y); }
            U1 neg(const U1 &x) const { return sub(0, x); }
        };

        bool is_prime(u128 n)
        {
            using U1 = u128;

            if (n < 2) return false;
            for (U1 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
                if (n == p) return true;
                if (n % p == 0) return false;
            }
            if (n < 41 * 41) return true;

            dynamic_mod<128> mont(n);
            const U1 one = mont.R1, neg_one = mont.neg(one);

            auto test_miller_rabin = [&](const std::vector<U1> &bases) {
                // int e = std::countr_zero(n - 1);
                // U1 o = n >> e;
                int e = 0;
                U1 o = n - 1;
                while (~o & 1) {
                    o >>= 1;
                    e++;
                }
                for (U1 b : bases) {
                    U1 x = mont.pow(mont.from(b), o);
                    if (x == one) continue;
                    for (int ei = 0; ei < e; ++ei) {
                        U1 y = mont.mul(x, x);
                        if (y == one) {
                            if (x == neg_one) break;
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
            if (n < 18446744073709551616_u128) return test_miller_rabin({2, 325, 9375, 28178, 450775, 9780504, 1795265022});
            if (n < 318665857834031151167461_u128) return test_miller_rabin({2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37});
            if (n < 3317044064679887385961981_u128) return test_miller_rabin({2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41});
            assert(false);
        }

        u128 gcd(u128 a, u128 b)
        {
            while (b) {
                u128 r = a % b;
                std::tie(a, b) = {b, r};
            }
            return a;
        }

        template <int ROUND = 1 << 12>
        std::vector<u128> factorize(u128 n)
        {
            using std::array;
            using std::vector;
            if (n == 1) return {};
            if (is_prime(n)) return {n};

            vector<u128> ans;
            for (u128 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
                while (n % p == 0) {
                    ans.push_back(p);
                    n /= p;
                }
            }
            if (n == 1) return ans;
            // here n is odd, n >= 3 (or n > 37)
            auto abs_diff = [](u128 x, u128 y) { return x > y ? x - y : y - x; };

            auto main = [&](auto self, u128 nn) -> vector<u128> {
                if (nn == 1) return {};
                if (is_prime(nn)) return {nn};
                using namespace io;
                dynamic_mod<128> mont(nn);

                auto find_factor = [&]() {
                    u128 rc = 0;
                    auto f = [&](u128 rx) { return mont.add(mont.mul(rx, rx), rc); };

                    while (true) {
                        ++rc;
                        u128 rx = rc, ry = rx;
                        u128 d = 1;

                        // round ごとにみる
                        array<u128, 2> checkpoint{rx, ry};
                        while (d == 1) {
                            u128 combined = 1;
                            for (u128 _ = 0; _ < ROUND; _++) {
                                rx = f(rx);
                                ry = f(f(ry));
                                combined = mont.mul(combined, abs_diff(rx, ry));
                            }
                            d = gcd(combined, nn);
                            if (d == 1) {
                                // この round では見つからず
                                checkpoint = {rx, ry};
                            }
                            if (d != 1 && d != nn) return d;
                        }

                        // 1つずつみる
                        // tie(rx, ry) = checkpoint;
                        rx = checkpoint[0];
                        ry = checkpoint[1];
                        d = 1;
                        while (d == 1) {
                            rx = f(rx);
                            ry = f(f(ry));
                            d = gcd(abs_diff(rx, ry), nn);
                            if (d != 1 && d != nn) return d;
                        }
                        // if (d == nn) continue;
                        // return d;
                    }
                };
                u64 d = find_factor();
                vector a1 = self(self, d);
                vector a2 = self(self, nn / d);
                a1.insert(a1.end(), a2.begin(), a2.end());
                return a1;
            };
            vector a = main(main, n);
            ans.insert(ans.end(), a.begin(), a.end());
            return ans;
        }

        std::vector<std::pair<u128, int>> to_pairs(std::vector<u128> primes)
        {
            std::ranges::sort(primes);
            std::vector<std::pair<u128, int>> pairs;
            for (u128 p : primes) {
                if (!pairs.empty() && pairs.back().first == p) {
                    pairs.back().second++;
                } else {
                    pairs.emplace_back(p, 1);
                }
            }
            return pairs;
        }

        // [d for d divides n if pred(d)]
        // pred(d) == true  <==>  small enough
        template <typename T>
        requires requires(T pred, u128 l) {
            { pred(l) } -> std::same_as<bool>;
        }
        std::vector<u128> list_divisors(const std::vector<std::pair<u128, int>> &pe, T pred)
        {
            if (!pred(1)) return {};
            std::vector<u128> ans{1};
            for (auto [p, e] : pe) {
                for (int i = 0, s = ans.size(); i < s; ++i) {
                    u128 d = ans[i];
                    for (int ei = 1; ei <= e; ++ei) {
                        d *= p;
                        if (!pred(d)) break;
                        ans.push_back(d);
                    }
                }
            }
            return ans;
        }
    }  // namespace factorization

    inline namespace solver_util
    {
        // e-th power sum of [l, r) or [0, l)
        u128 power_sum(int e, u128 l, u128 r = 0)
        {
            if (r == 0) {
                r = l;
                l = 0;
            }
            u128 w = r - l;
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

        u128 saturating_pow(u128 a, int e)
        {
            u128 ans = 1;
            for (int i = 0; i < e; ++i) {
                if (__builtin_mul_overflow(ans, a, &ans)) return std::numeric_limits<u128>::max();
            }
            return ans;
        }

        // min i in [lo, hi) s.t. pred(i), or default = hi
        template <typename T>
        requires requires(T pred, u128 l) {
            { pred(l) } -> std::same_as<bool>;
        }
        u128 bisect_left(u128 lo, u128 hi, T pred)
        {
            if (pred(lo)) return lo;
            while (lo + 1 < hi) {
                // u128 mi = std::midpoint(lo, hi);
                u128 mi = (lo + hi) >> 1;
                if (pred(mi)) {
                    hi = mi;
                } else {
                    lo = mi;
                }
            }
            return hi;
        }
    }  // namespace solver_util

    namespace solver
    {
        template <typename T>
        using solution_t = std::tuple<int, T, T>;

        template <typename T>
        struct named_solver_base {
            std::string name;
            virtual std::vector<solution_t<T>> operator()(T n, int e) const
            {
                using namespace io;
                std::cerr << "virtual function called(" << n << "," << e << ")" << std::endl;
                return {};
            }
            const named_solver_base &get() const { return *this; }
        };
        template <typename T, typename F>
        struct named_solver : named_solver_base<T> {
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
                for (int e = E_FIRST + count; e < std::numeric_limits<T>::digits; ++e) {
                    auto ansi = fs[count - 1](n, e);
                    ans.insert(ans.end(), ansi.begin(), ansi.end());
                }
                return ans;
            }
        };

        template <typename T>
        std::vector<T> read_all()
        {
            using namespace io;
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

        void answer_cubic_with(const wrapped_solver<lint> &f)
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
        void answer_power_with(const combined_solver<T, E_FIRST> &f)
        {
            using std::cout;
            using namespace io;
            auto ns = read_all<T>();
            for (T n : ns) {
                auto ans = f(n);
                cout << ans.size() << '\n';
                for (auto [e, l, r] : ans) {
                    cout << e << ' ' << l << ' ' << r << '\n';
                }
            }
        }
    }  // namespace solver

    std::vector<int> parse_range(int size, const std::string &line)
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
