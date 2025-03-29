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

    auto gen_cases = [](int num, auto f) {
        auto cases = views::repeat(0, num) | views::transform([&](auto) { return f(); }) | ranges::to<vector<lint>>();
        return cases;
    };
    auto dump_cases = [](string prefix, vector<lint> cases, bool sort = true) {
        if (sort) ranges::sort(cases);
        for (auto [i, n] : cases | views::enumerate) {
            int t = i;
            ofstream ofs(format("%s-%02d.in", prefix.c_str(), t).c_str());
            ofs << n << endl;
            ofs.close();
        }
    };
    dump_cases("01-random"s, gen_cases(15, gen_have_sol));
    dump_cases("02-random"s, gen_cases(15, gen_reciprocal));
    dump_cases("10-handmade"s, {2, 5, 7, 999999'999999'999967, 999999'999999'999989});
    dump_cases("11-handmade"s, {1, 8, 9, 999693664352352684, 999844743676712327, 999999'999043'722144, 999999'999136'704159, 1'000000'000000'000000});
    dump_cases("12-handmade"s, {8000, 999116'559748'794375, 999563'958497'309400});
    dump_cases("13-handmade"s, {23295'638016});
}
