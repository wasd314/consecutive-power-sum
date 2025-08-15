#! /usr/bin/env python
import sys
from pathlib import Path
sol = Path(__file__).resolve().parents[2] / "solution_py"
sys.path.append(str(sol))

import e2
import ee
import utility
import random
from consts import MIN_N, MAX_N

random.seed("consecutive power sum (more)")

# max c s.t. c[e]**e <= MAX_N
c = {e: utility.min_true(1, MAX_N, lambda n: n**e > MAX_N) - 1 for e in range(1, 100)}

def gen_reciprocal(min_n: int, max_n: int):
    """一様乱択よりも（大雑把に）小さい数の出現確率を上げて乱択"""
    bit = max_n.bit_length()
    while True:
        n = random.randrange(0, 1 << bit) >> random.randrange(0, bit)
        if min_n <= n <= max_n:
            return n

def gen_have_sol(min_n: int, max_n: int):
    bit = max_n.bit_length()
    while True:
        e = random.randint(1, bit)
        l = random.randint(1, c[e])
        r = random.randint(1, c[e]) + 1
        n = utility.power_sum(e, l, r)
        if l <= r and min_n <= n <= max_n:
            return n

def gen_cases(num: int, *fs):
    return [f() for f in fs for _ in range(num)]

def number_of_solutions(n: int):
    return len(ee.re1_bs_div_pre(n, 4))

def dump_cases(prefix, cases: list, sort=True):
    buckets = [[] for _ in range(5)]
    if sort:
        cases = sorted(cases, key=lambda n: (number_of_solutions(n), n))
    for n in cases:
        if n <= 10**18:
            buckets[1].append(n)
        elif n <= 10**22:
            buckets[2].append(n)
        elif n <= 10**23:
            buckets[3].append(n)
        elif n <= 10**24:
            buckets[4].append(n)
        else:
            assert False
    for bi, bucket in enumerate(buckets):
        for i, n in enumerate(bucket):
            with open(f"{bi}{prefix}-{i:02}.in", "w") as f:
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

dump_cases("0-handmade", handmade)

dump_cases("1-random", gen_cases(20, lambda: gen_have_sol(MIN_N, MAX_N)))
dump_cases("2-random", gen_cases(10, lambda: gen_reciprocal(MIN_N, MAX_N), lambda: gen_reciprocal(10**18, MAX_N)))

