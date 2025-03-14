#include <iostream>
#include <random>

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

    for (int t = 0; t < 100; ++t) {
        ofstream ofs(format("02-random-%02d.in", t).c_str());
        lint n = gen_reciprocal();
        ofs << n << endl;
        ofs.close();
    }
}
