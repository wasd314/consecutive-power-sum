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

all_cases = set()

def gen_cases(num: int, *fs):
    cases = []
    for i, f in enumerate(fs, 1):
        while len(cases) < i * num:
            n = f()
            if n not in all_cases and n not in cases:
                cases.append(n)
    return cases

def dump_cases(prefix, cases: list[int], uniq_sort=True):
    buckets = [[] for _ in range(5)]
    if uniq_sort:
        cases = sorted(
            {n for n in cases if MIN_N <= n <= MAX_N and n not in all_cases},
            key=lambda n: (e1.number_of_solutions(n), n)
        )
    all_cases.update(cases)
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
handmade = [3, 4, 5, 6, 7, 8, 9]
# limit
handmade.extend([10**12])
# power
handmade.extend([2**39 + delta for delta in (-1, 0, 1)])
handmade.extend([3**25 + delta for delta in (-1, 0, 1)])
handmade.extend([3**25 + 2**25 + delta for delta in (-1, 0, 1)])
# prime
handmade.extend([10**12 - 11, 10**12 - 39])
# semiprime n = pq
# - q-p ~ 1e5 (p % 12 = q % 12 = 5, 7)
handmade.extend([950933 * 1050773, 935107 * 1061707])
# - q-p ~ 4e4
handmade.extend([974837 * 1022837, 978883 * 1018987])
# - p = q: n = p^2
handmade.extend([999983**2, 999979**2])
# highly composite
# d(2n, sqrt(2n)) = 3456, 3600, 3840
# 985496152350226952635200: also argmax d(6n, sqrt[3](3n)), argmax d(2n, sqrt[4](4n))
handmade.extend([642507465600, 843291048600, 963761198400])
# odd highly composite
# d(2n, sqrt(2n)) = d(n, n) = 1920, 2048
handmade.extend([436704293025, 727840488375])
# primorial p_11#
handmade.extend([200560490130])
# hack fixed Pollard's rho (using lambda x: x*x+c, where c = 1)
handmade.extend([352523 * 352817])


dump_cases("0_handmade", handmade)

dump_cases("1_random", gen_cases(10, lambda: gen_have_sol(MIN_N, MAX_N)))
dump_cases("2_random", gen_cases(10, lambda: gen_reciprocal(MIN_N, MAX_N)))

