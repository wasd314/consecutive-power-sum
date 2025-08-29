#! /usr/bin/env pypy
import sys
from pathlib import Path
sol = Path(__file__).resolve().parents[2] / "solution_py"
sys.path.append(str(sol))

import e1
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

def dump_cases(prefix, cases: list[int], uniq_sort=True):
    buckets = [[] for _ in range(5)]
    if uniq_sort:
        cases = sorted(
            {n for n in cases if MIN_N <= n <= MAX_N},
            key=lambda n: (e1.number_of_solutions(n), n)
        )
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
            with open(f"{bi}{prefix}_{i:02}.in", "w") as f:
                print(n, file=f)


# small
handmade = [2, 5, 7, 8, 9]
# have solution
handmade.extend([8000, 23295_638016])
# limit
handmade.extend([10**22, 10**23, 10**24])
# power
handmade.extend([2**e + delta for e in (73, 76, 79) for delta in (-1, 0, 1)])
handmade.extend([3**e for e in (46, 48, 50)])
# prime
handmade.extend([10**22 - 27, 10**23 - 23, 10**24 - 257])
# semiprime n = pq (q/p ~ 10, ~ 1000)
handmade.extend([10**22 - 41, 10**23 - 587, 10**24 - 1077])
handmade.extend([10**22 - 83, 10**23 - 503, 10**24 - 1901])
# highly composite
# d(2n, sqrt(2n)) = 322560, 491520, 737280
# 985496152350226952635200: also argmax d(6n, sqrt[3](3n)), argmax d(2n, sqrt[4](4n))
handmade.extend([9297133512737990119200, 92971335127379901192000, 985496152350226952635200])
# odd highly composite
# d(2n, sqrt(2n)) = 165888, 245760, 368640
handmade.extend([9587668935011052310425, 76991886902361480674625, 846910755925976287420875])
# hack deterministic Miller–Rabin
# - hack {2, 325, 9375, 28178, 450775, 9780504, 1795265022} (at least 2^64)
handmade.extend([62119104158988074251, 164959812840562904431, 2555929540142715989071])
# - hack {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}
handmade.extend([318665857834031151167461])


dump_cases("0_handmade", handmade)

dump_cases("1_random", gen_cases(10, lambda: gen_have_sol(MIN_N, MAX_N)))
dump_cases("2_random", gen_cases(10, lambda: gen_reciprocal(MIN_N, MAX_N), lambda: gen_reciprocal(10**20, MAX_N)))

