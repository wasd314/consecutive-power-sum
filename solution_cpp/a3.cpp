#include <array>
#include <iostream>
#include <vector>

#include "e3.hpp"

int main()
{
    using namespace wasd314::e3;
    wasd314::answer_cubic_with(r30_two_pointer);
    wasd314::answer_power_with<1>({solvers[0]});
}
