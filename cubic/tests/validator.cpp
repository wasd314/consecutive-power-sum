
#include <iostream>
#include <random>

#include "consts.hpp"
#include "testlib.h"

int main(int argc, char **argv)
{
    using namespace std;
    using lint = long long;
    registerValidation(argc, argv);
    inf.readLong(MIN_N, MAX_N, "N");
    inf.readEoln();
    inf.readEof();
}
