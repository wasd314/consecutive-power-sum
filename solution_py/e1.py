import ee
from utility import CombinedSolver as cs
import sympy as sp


def r13(n: int, e: int):
    ans = []
    for w in sp.divisors(2 * n):
        w2 = 2 * n // w
        if w > w2 or w % 2 == w2 % 2:
            continue
        l = (w2 - w + 1) // 2
        ans.append((1, l, l + w - 1))
    ans.sort()
    return ans

def number_of_e1_solutions(n: int):
    while n % 2 == 0:
        n //= 2
    return int(sp.divisor_count(n))

def number_of_solutions(n: int):
    ans = 0
    # e = 1
    ans += number_of_e1_solutions(n)
    # e >= 2
    e2_solver = cs(2, ee.re1[5], ee.re1[5], ee.re0_two_pointer)
    ans += len(e2_solver(n))
    return ans

solvers_ac = [
    cs(1, r13, ee.re1[2], ee.re1[2], ee.re0_two_pointer),
    cs(1, r13, ee.re1[3], ee.re1[3], ee.re0_two_pointer),
    cs(1, r13, ee.re1[4], ee.re1[4], ee.re0_two_pointer),
    cs(1, r13, ee.re1[5], ee.re1[5], ee.re0_two_pointer),
]

