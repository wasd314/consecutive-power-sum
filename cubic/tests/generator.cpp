#include <cmath>
#include <iostream>
#include <random>
#include <ranges>

#include "consts.hpp"
#include "testlib.h"

int main(int argc, char **argv)
{
    using namespace std;
    using lint = long long;
    registerGen(argc, argv, 1);
    std::mt19937_64 rng{};

    uniform_int_distribution<int> exponent(0, 60);
    uniform_int_distribution<lint> mantissa(0, (1ll << 60) - 1);
    auto gen_reciprocal = [&]() {
        // approximately reciprocal distribution
        while (true) {
            lint n = mantissa(rng);
            n >>= exponent(rng);
            if (MIN_N <= n && n <= MAX_N) {
                return n;
            }
        }
    };

    vector<__int128_t> acc{0};
    acc.reserve(int(cbrtl(MAX_N)) + 10);
    for (lint i = 0; i * i * i <= MAX_N; ++i) {
        acc.push_back(acc.back() + i * i * i);
    }
    const int c = acc.size() - 2;
    // c**3 まである
    uniform_int_distribution<int> lw(1, c);

    auto gen_have_sol = [&]() -> lint {
        while (true) {
            int l = lw(rng);
            int w = lw(rng);
            int r = l + w - 1;
            if (r <= c && acc[r + 1] - acc[l] <= MAX_N) {
                return acc[r + 1] - acc[l];
            }
        }
    };

    auto cases01 = views::repeat(0, 20) | views::transform([&](auto) { return gen_have_sol(); }) | ranges::to<vector<lint>>();
    ranges::sort(cases01);
    auto cases02 = views::repeat(0, 20) | views::transform([&](auto) { return gen_reciprocal(); }) | ranges::to<vector<lint>>();
    ranges::sort(cases02);

    for (auto [i, n] : cases01 | views::enumerate) {
        int t = i;
        ofstream ofs(format("01-random-%02d.in", t).c_str());
        ofs << n << endl;
        ofs.close();
    }
    for (auto [i, n] : cases02 | views::enumerate) {
        int t = i;
        ofstream ofs(format("02-random-%02d.in", t).c_str());
        ofs << n << endl;
        ofs.close();
    }
}
