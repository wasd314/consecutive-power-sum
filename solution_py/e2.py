import ee
from utility import CombinedSolver as cs

solvers_ac = [
    cs(2, ee.re1[0], ee.re0_two_pointer),
    cs(2, ee.re1[1], ee.re0_two_pointer),
    cs(2, ee.re1[2], ee.re0_two_pointer),
    cs(2, ee.re1[3], ee.re0_two_pointer),
]

default = solvers_ac[3]

solvers_tle = [
    cs(2, ee.re0[0], ee.re0_two_pointer),
    cs(2, ee.re0[1], ee.re0_two_pointer),
    cs(2, ee.re0[2], ee.re0_two_pointer),
]
