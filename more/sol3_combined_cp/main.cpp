#include <algorithm>
#include <bit>
#include <cassert>
#include <concepts>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <ranges>
#include <sstream>
#include <type_traits>
#include <vector>

namespace wasd314
{
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
        constexpr u128 operator"" _u128(const char *s) { return io::parse_u128(s); }
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
        struct dynamic_mod4 {
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
            dynamic_mod4(const U1 &modulo) : mod(modulo)
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

            dynamic_mod4<128> mont(n);
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
                dynamic_mod4<128> mont(nn);

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
        using solution_t = std::tuple<int, u128, u128>;

        std::vector<solution_t> r13_pe(u128 n, int)
        {
            using namespace std;
            using namespace factorization;
            vector<solution_t> ans;
            auto pe_2n = to_pairs(factorize(2 * n));
            auto ds = list_divisors(pe_2n, [&](u128 d) { return d <= (2 * n - 1) / d; });
            // auto ds = list_divisors(pe_2n, [&](u128 d) { return __int128_t(d) * d < 2 * n; });
            for (u128 w : ds) {
                u128 w2 = 2 * n / w;
                assert(w < w2);
                if (w % 2 == w2 % 2) continue;
                u128 l = (w2 - w + 1) / 2;
                ans.emplace_back(1, l, l + w - 1);
            }
            ranges::sort(ans);
            return ans;
        }

        std::vector<solution_t> re0_two_pointer(u128 n, int e)
        {
            using std::vector;
            // using wasd314::solver_util::saturating_pow;
            vector<u128> pows;
            for (u128 i = 0;; ++i) {
                u128 ip = saturating_pow(i, e);
                if (ip > n) break;
                pows.push_back(ip);
            }
            int c = pows.size();

            vector<solution_t> ans;
            int r = 1;
            u128 current_sum = 0;
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

        // min D s.t. W divides [D * S(e, L, L+W-1)] for all L & W.
        const int enough_denom[] = {1, 2, 6, 2, 30, 2, 42, 2, 30, 2, 66, 2, 2730, 2, 6, 2, 510, 2, 798, 2, 330, 2, 138, 2, 2730, 2, 6, 2, 870, 2, 14322, 2, 510, 2, 6, 2, 1919190, 2, 6, 2, 13530, 2, 1806, 2, 690, 2, 282, 2, 46410, 2, 66, 2, 1590, 2, 798, 2, 870, 2, 354, 2, 56786730, 2, 6, 2, 510, 2, 64722, 2, 30, 2, 4686, 2, 140100870, 2, 6, 2, 30, 2, 3318, 2, 230010, 2, 498, 2, 3404310, 2, 6, 2, 61410, 2, 272118, 2, 1410, 2, 6, 2, 4501770, 2, 6, 2};

        template <bool ONLY_DIV, bool FROM_PREV>
        std::vector<solution_t> re1_bs(u128 n, int e)
        {
            using solver_util::bisect_left;
            using std::vector;
            vector<solution_t>
                ans;
            u128 prev_l = n;
            for (u128 w = 1;; ++w) {
                if (power_sum(e, 1, w + 1) > n) break;
                if (ONLY_DIV && (enough_denom[e] * n % w) != 0) continue;
                u128 l;
                auto pred = [&](u128 li) { return power_sum(e, li, li + w) >= n; };
                if (FROM_PREV && w != 1) {
                    u128 dl = 1;
                    while (dl < prev_l && pred(prev_l - dl)) {
                        dl <<= 1;
                    }
                    l = bisect_left(std::max(0_u128, prev_l - dl), prev_l, pred);
                } else {
                    u128 r = 1;
                    while (!pred(r)) {
                        r <<= 1;
                    }
                    l = bisect_left(0, r, pred);
                }
                if (power_sum(e, l, l + w) == n) {
                    ans.emplace_back(e, l, l + w - 1);
                }
                prev_l = l;
            }
            std::ranges::reverse(ans);
            // std::ranges::sort(ans);
            return ans;
        }

        void solve(u128 n)
        {
            using std::vector;
            using namespace io;
            vector<vector<solution_t>> answers;
            answers.push_back(r13_pe(n, 1));
            answers.push_back(re1_bs<true, true>(n, 2));
            answers.push_back(re1_bs<true, true>(n, 3));
            answers.push_back(re1_bs<true, true>(n, 4));
            for (int e = 5; (n >> e) != 0; ++e) {
                answers.push_back(re0_two_pointer(n, e));
            }
            vector<solution_t> ans;
            for (const auto &ans_e : answers) {
                ans.insert(ans.end(), ans_e.begin(), ans_e.end());
            }
            std::stringstream buf;
            buf << ans.size() << '\n';
            for (auto &[e, l, r] : ans) {
                buf << e << ' ' << l << ' ' << r << '\n';
            }
            std::cout << buf.str();
        }
    }  // namespace solver
}  // namespace wasd314

int main()
{
    using namespace std;
    using namespace wasd314;
    using namespace wasd314::io;
    // using namespace wasd314::literals;
    // using namespace wasd314::factorization;
    u128 n;
    cin >> n;
    solver::solve(n);
}
