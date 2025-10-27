#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <ranges>
#include <sstream>
#include <type_traits>
#include <vector>

namespace wasd314
{
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
        struct integer_pack<128> {
            using I0 = i64;
            using U0 = u64;
            using I1 = i128;
            using U1 = u128;
            static constexpr int BITS_0 = std::numeric_limits<U0>::digits;
            static constexpr int BITS_1 = std::numeric_limits<U1>::digits;
        };

        // MOD_BITS bit 符号なし整数で mod を取る
        // Montgomery reduction による
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

        // 素数判定する
        // Miller–Rabin により Θ(log n)
        bool is_prime(u128 n)
        {
            if (n < 2) return false;
            for (u128 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
                if (n == p) return true;
                if (n % p == 0) return false;
            }
            if (n < 41 * 41) return true;

            dynamic_mod<128> mont(n);
            const u128 one = mont.R1, neg_one = mont.neg(one);

            auto test_miller_rabin = [&](const std::initializer_list<u128> &bases) {
                int e = 0;
                u128 o = n - 1;
                while (~o & 1) {
                    o >>= 1;
                    e++;
                }
                for (u128 b : bases) {
                    u128 x = mont.pow(mont.from(b), o);
                    if (x == one || x == neg_one) continue;
                    for (int ei = 1; ei < e; ++ei) {
                        x = mont.mul(x, x);
                        if (x == neg_one) break;
                    }
                    if (x != neg_one) return false;
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

        // 素因数分解する
        // Pollard's rho において gcd を round ∈ Θ(n^{1/8}) 回ごとにとり expected O(n^{1/4}) 時間
        std::vector<u128> factorize(u128 n)
        {
            if (n == 1) return {};
            if (is_prime(n)) return {n};

            std::vector<u128> ans;
            for (u128 p : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
                while (n % p == 0) {
                    ans.push_back(p);
                    n /= p;
                }
            }
            if (n == 1) return ans;
            // here n is odd, n >= 3 (or n > 37)

            auto abs_diff = [](u128 x, u128 y) { return x > y ? x - y : y - x; };
            // calc around x^{1/8}
            auto calc_round = [](u128 x) {
                int i = 0;
                while (x >> (8 * i)) i++;
                // since x < 2^128, x^{1/8} < 2^16 < 2^31
                return 1 << i;
            };

            auto main = [&](auto self, u128 nn) -> std::vector<u128> {
                if (nn == 1) return {};
                if (is_prime(nn)) return {nn};
                dynamic_mod<128> mont(nn);
                const int round = calc_round(nn);
                // 非自明な因数を1つ返す
                auto find_factor = [&] {
                    u128 rc = 0;
                    auto f = [&](u128 rx) { return mont.add(mont.mul(rx, rx), rc); };
                    while (true) {
                        ++rc;
                        u128 rx = rc, ry = rx;
                        u128 d = 1;

                        // round ごとにみる
                        std::pair<u128, u128> checkpoint{rx, ry};
                        while (d == 1) {
                            u128 combined = 1;
                            for (int _ = 0; _ < round; _++) {
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
                        std::tie(rx, ry) = checkpoint;
                        d = 1;
                        while (d == 1) {
                            rx = f(rx);
                            ry = f(f(ry));
                            d = gcd(abs_diff(rx, ry), nn);
                            if (d != 1 && d != nn) return d;
                        }
                    }
                };
                u128 d = find_factor();
                std::vector a1 = self(self, d);
                std::vector a2 = self(self, nn / d);
                a1.insert(a1.end(), a2.begin(), a2.end());
                return a1;
            };
            std::vector a = main(main, n);
            ans.insert(ans.end(), a.begin(), a.end());
            return ans;
        }

        // 頻度分布にする
        std::map<u128, int> to_freq(const std::vector<u128> &primes)
        {
            std::map<u128, int> freq;
            for (u128 p : primes) {
                freq[p]++;
            }
            return freq;
        }

        // `[d for d divides n if pred(d)]`.
        // ただし順序は不定
        // `pred` について ∃x. (pred(d) == true  <==>  d < x) を仮定
        template <typename T>
        requires requires(T pred, u128 l) {
            { pred(l) } -> std::same_as<bool>;
        }
        std::vector<u128> list_divisors(const std::map<u128, int> &pe, T pred)
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
        u128 min_true(u128 lo, u128 hi, T pred)
        {
            if (pred(lo)) return lo;
            while (lo + 1 < hi) {
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
        using solution_t = std::tuple<int, u128, u128>;

        // e = 1 の解を列挙する
        // 素因数分解を除いて d(2n, (2n)^{1/2}) ) 時間
        std::vector<solution_t> r13_pe(u128 n, int, const std::map<u128, int> &pe_n)
        {
            using namespace factorization;
            std::vector<solution_t> ans;
            // 2 * n の素因数分解
            std::map pe_2n = pe_n;
            pe_2n[2]++;

            // d*d < 2*n
            std::vector ds = list_divisors(pe_2n, [&](u128 d) { return d <= (2 * n - 1) / d; });
            for (u128 w : ds) {
                u128 w2 = 2 * n / w;
                assert(w < w2);
                if (w % 2 == w2 % 2) continue;
                u128 l = (w2 - w + 1) / 2;
                ans.emplace_back(1, l, l + w - 1);
            }
            std::ranges::sort(ans);
            return ans;
        }

        // min D s.t. W divides [D * S(e, L, L+W-1)] for all L & W.
        const int enough_denom[] = {1, 2, 6, 2, 30, 2, 42, 2, 30, 2, 66, 2, 2730, 2, 6, 2, 510, 2, 798, 2, 330, 2, 138, 2, 2730, 2, 6, 2, 870, 2, 14322, 2, 510, 2, 6, 2, 1919190, 2, 6, 2, 13530, 2, 1806, 2, 690, 2, 282, 2, 46410, 2, 66, 2, 1590, 2, 798, 2, 870, 2, 354, 2, 56786730, 2, 6, 2, 510, 2, 64722, 2, 30, 2, 4686, 2, 140100870, 2, 6, 2, 30, 2, 3318, 2, 230010, 2, 498, 2, 3404310, 2, 6, 2, 61410, 2, 272118, 2, 1410, 2, 6, 2, 4501770, 2, 6, 2};

        // e = e （小さい）の解を列挙する
        // 二分探索により Θ(d(D_e N, W_e) log N) 時間
        std::vector<solution_t> re1_bisect_div(u128 n, int e, const std::map<u128, int> &pe_n)
        {
            using namespace factorization;
            using solver_util::min_true;
            // enough_denom[e] * n の素因数分解
            std::map pe_dn = pe_n;
            for (u128 p : factorize(enough_denom[e])) pe_dn[p]++;

            std::vector divisors = list_divisors(pe_dn, [&](u128 w) { return power_sum(e, 1, w + 1) <= n; });
            std::vector<solution_t> ans;
            u128 prev_l = n;
            for (u128 w : divisors) {
                auto pred = [&](u128 li) { return power_sum(e, li, li + w) >= n; };
                u128 r = 1;
                while (!pred(r)) {
                    r <<= 1;
                }
                u128 l = min_true(0, r, pred);
                if (power_sum(e, l, l + w) == n) {
                    ans.emplace_back(e, l, l + w - 1);
                }
                prev_l = l;
            }
            std::ranges::sort(ans);
            return ans;
        }

        // e = e （大きい）の解を列挙する
        // 尺取り法により Θ(n^{1/e}) 時間
        std::vector<solution_t> re0_two_pointer(u128 n, int e)
        {
            std::vector<u128> pows;
            for (u128 i = 0;; ++i) {
                u128 ip = saturating_pow(i, e);
                if (ip > n) break;
                pows.push_back(ip);
            }
            int m = pows.size();

            std::vector<solution_t> ans;
            int r = 1;
            u128 current_sum = 0;
            for (int l = 1; l < m; ++l) {
                while (r < m && current_sum + pows[r] <= n) {
                    current_sum += pows[r];
                    r++;
                }
                if (current_sum == n) {
                    ans.emplace_back(e, l, r - 1);
                }
                current_sum -= pows[l];
            }
            return ans;
        }

        void solve(u128 n)
        {
            using namespace io;
            using namespace factorization;
            std::vector<std::vector<solution_t>> answers;
            // n の素因数分解
            // 一度求めて使い回す
            std::map pe_n = to_freq(factorize(n));

            answers.push_back(r13_pe(n, 1, pe_n));
            for (int e = 2; (n >> e) != 0; ++e) {
                if (e <= 3) {
                    answers.push_back(re1_bisect_div(n, e, pe_n));
                } else {
                    answers.push_back(re0_two_pointer(n, e));
                }
            }

            std::vector<solution_t> ans;
            for (auto &ans_e : answers) {
                std::ranges::sort(ans_e);
                ans.insert(ans.end(), ans_e.begin(), ans_e.end());
            }

            std::stringstream buf;
            buf << ans.size() << '\n';
            for (auto [e, l, r] : ans) {
                buf << e << ' ' << l << ' ' << r << '\n';
            }
            std::cout << buf.str();
        }
    }  // namespace solver
}  // namespace wasd314

int main()
{
    using namespace wasd314;
    using namespace wasd314::io;
    u128 n;
    std::cin >> n;
    assert(2_u128 <= n && n <= 1'000'000'000'000'000'000'000'000_u128);
    wasd314::solver::solve(n);
}
