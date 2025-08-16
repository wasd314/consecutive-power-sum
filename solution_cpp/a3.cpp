#include <array>
#include <iostream>
#include <set>
#include <vector>

#include "e1.hpp"
#include "e3.hpp"

int main(int argc, char **argv)
{
    using namespace std;
    using namespace wasd314;
    using namespace wasd314::io;
    // wasd314::answer_cubic_with(wasd314::ee::re0_two_pointer);
    // int i = std::stoi(argv[1]);
    // std::cout << i << std::endl;
    // wasd314::combined_solver<2> cs = {solvers[i], solvers[0]};
    // std::cout << cs.name << std::endl;
    // wasd314::answer_power_with(cs);
    // wasd314::answer_power_with<2>({solvers[0], solvers[0]});
    // wasd314::answer_power_with<1>({e1::r13_pe, ee::re1[3], ee::re1[3], ee::re1[3], ee::re0[0]});
    // wasd314::answer_power_with<2>({ee::re1[3], ee::re1[3], ee::re1[3], ee::re0[0]});
    solver::combined_solver<u128, 1> cs{e1::r13_pe, ee::re1<u128>[3], ee::re1<u128>[3], ee::re1<u128>[3], ee::re0<u128>[0]};
    // cout << cs.name << endl;
    int found = 0;
    for (lint n = 2; n < 10000000 && found < 10; n++) {
        auto ans = cs(n);
        set<int> es;
        for (auto [e, l, r] : ans) es.insert(e);
        // if (es.size() < 3  || es.size() == ans.size() || ans.size() > es.size() + 3) continue;
        if (es.size() == 1 || get<0>(ans.back()) == 2 || ans.size() != es.size() + 1) continue;
        found++;
        cout << n << " " << ans.size() << endl;
        for (auto [e, l, r] : ans) {
            cout << "\t" << e << " " << l << " " << r << endl;
        }
    }
    // answer_cubic_with(e1::r13_pe);
    // answer_power_with(cs);
}
