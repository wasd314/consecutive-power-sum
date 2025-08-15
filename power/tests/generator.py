#! /usr/bin/env python
import sys
from pathlib import Path
sol = Path(__file__).resolve().parents[2] / "solution_py"
sys.path.append(str(sol))

import e2
import utility
import random
import bisect as bs

random.seed("consecutive power sum")

MIN_N = 2
MAX_N = 10**18
# max c s.t. c[e]**e <= MAX_N
c = {e: bs.bisect_right(range(MAX_N + 1), MAX_N, key=lambda x: x**e) - 1 for e in range(1, 64)}

def gen_reciprocal(min_n: int, max_n: int):
    """一様乱択よりも（大雑把に）小さい数の出現確率を上げて乱択"""
    while True:
        n = random.randrange(0, 1 << 60) >> random.randrange(0, 60)
        if min_n <= n <= max_n:
            return n

def gen_have_sol(min_n: int, max_n: int):
    while True:
        e = random.randint(2, 63)
        l = random.randint(1, c[e])
        r = random.randint(1, c[e]) + 1
        n = utility.power_sum(e, l, r)
        if l <= r and min_n <= n <= max_n:
            return n

def gen_cases(num: int, f):
    return [f() for _ in range(num)]

def number_of_solutions(n: int):
    return len(e2.default(n))

def dump_cases(prefix, cases: list, sort=True):
    if sort:
        cases = sorted(cases, key=lambda n: (number_of_solutions(n), n))
    for i, n in enumerate(cases):
        with open(f"{prefix}-{i:02}.in", "w") as f:
            print(n, file=f)

# small
handmade = [2, 5, 7, 8, 9]
# have solution
handmade.extend([8000, 23295_638016])
handmade.extend([999693_664352_352684, 999844_743676_712327, MAX_N])
# have 2 solutions
handmade.extend([999116_559748_794375, 999563_958497_309400])
# prime
handmade.extend([999999_999999_999967, 999999_999999_999989])
# semiprime
handmade.extend([999999_999999_999787, 999999_999999_999541])

dump_cases("10-handmade", handmade)

dump_cases("11-random", gen_cases(20, lambda: gen_have_sol(MIN_N, MAX_N)))
dump_cases("12-random", gen_cases(10, lambda: gen_reciprocal(MIN_N, MAX_N)))

