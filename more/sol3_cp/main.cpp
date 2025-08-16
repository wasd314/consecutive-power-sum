#include "e1.hpp"
#include "ee.hpp"
#include "utility.hpp"

int main()
{
    using namespace wasd314;
    solver::answer_power_with<u128, 1>({e1::r13_pe, ee::re1<u128>[3], ee::re1<u128>[3], ee::re0<u128>[0]});
}
