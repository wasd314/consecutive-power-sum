#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <queue>
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
        constexpr u128 operator""_u128(const char *s) { return io::parse_u128(s); }
    }  // namespace literals

}  // namespace wasd314

int main()
{
    using namespace std;
    using namespace wasd314;
    using namespace wasd314::io;
    vector<u128> primes{
        2,
        3,
        5,
        7,
        11,
        13,
        17,
        19,
        23,
        29,
        31,
        37,
        41,
        43,
        47,
        53,
        59,
        61,
        67,
        71,
        73,
        79,
        83,
        89,
        97,
    };
    int ip;
    u128 l, r, multiple;
    // l 以上 r 以下の multiple の倍数のうち primes[ip]-smooth
    cin >> ip >> l >> r >> multiple;
    assert(0 <= ip && ip < (int)primes.size());

    auto now_str = [] {
        auto now = chrono::system_clock::now();
        chrono::zoned_time z_now{chrono::current_zone(), now};
        return format("[{}]", z_now);
    };
    cerr << now_str() << ' ' << ip << ' ' << l << ' ' << r << ' ' << multiple << '\n';

    // sum(i**2 for i <= r) * 6
    auto acc26 = [](u128 r) { return r * (r + 1) * (2 * r + 1); };
    // max {w : acc26(w) <= n6 }
    auto up_to = [&](u128 n6) {
        u128 l = 0, r = 1;
        while (acc26(r) <= n6) {
            r *= 2;
        }
        while (r - l > 1) {
            u128 mi = (r + l) / 2;
            if (acc26(mi) <= n6) {
                l = mi;
            } else {
                r = mi;
            }
        }
        return l;
    };

    auto count_ac = [&](u128 n) -> int {
        u128 up = up_to(n);
        if (up < 1) return 0;
        vector<u128> ds{1};
        for (u128 p : primes) {
            if (n % p) continue;
            int e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            for (int i = 0, s = ds.size(); i < s; ++i) {
                u128 d = ds[i];
                for (int ei = 1; ei <= e; ++ei) {
                    d *= p;
                    if (d > up) break;
                    ds.push_back(d);
                }
            }
        }
        return ds.size();
    };
    using P = pair<u128, u128>;
    priority_queue<P, vector<P>, greater<P>> ans;
    u64 count = 0;
    auto enumerate_smooth = [&](this auto self, int i, u128 n) {
        if (i == -1) {
            if (l <= n && n <= r) {
                ans.emplace(count_ac(n), n);
                while (ans.size() > 100) {
                    ans.pop();
                }
                count++;
                if (count % 100000 == 0) {
                    cerr << now_str() << ' ' << count << '\n';
                }
            }
            return;
        }
        auto p = primes[i];
        while (n <= r) {
            self(i - 1, n);
            n *= p;
        }
    };
    enumerate_smooth(ip, multiple);
    cerr << now_str() << ' ' << count << " found" << '\n';
    {
        int i = 0;
        while (!ans.empty()) {
            auto [c, n] = ans.top();
            ans.pop();
            cerr << i << ' ' << c << ' ' << n << '\n';
            i++;
        }
    }
}
