#! /usr/bin/env pypy
import sys
from pathlib import Path
sol = Path(__file__).resolve().parents[2] / "solution_py"
sys.path.append(str(sol))

import ee
import utility
from collections import Counter
from sympy import isprime as is_prime
from functools import cache

MAX_N = 10**24

# max c s.t. c[e]**e <= MAX_N
c = {e: utility.min_true(1, MAX_N, lambda n: n**e > MAX_N) - 1 for e in range(1, 100)}

def lex_large(n):
    return str(n) > "24979"

def within(n):
    return 2**64 < n <= MAX_N

sol2 = utility.CombinedSolver(2, ee.re1[5], ee.re1[5], ee.re0_two_pointer)

@cache
def solve(n):
    return sol2(n)

def sol_count_e(n):
    return Counter(t[0] for t in solve(n))

def have_sols(n):
    return len(sol_count_e(n)) >= 2

def good1(n):
    return is_prime(n) and lex_large(n) and within(n)

w_max = 10
def search(e, *pred):
    # 1^e, 2^e, ..., nn^e
    nn = c[e]
    acc = [0] * (nn + 1)
    for i in range(nn):
        acc[i + 1] = acc[i] + (i + 1)**e

    good = []
    for w in range(1, w_max + 1):
        for r in range(w, nn + 1):
            l = r - w
            n = acc[r] - acc[l]
            if all(f(n) for f in pred):
                good.append(n)
                if (len(good) & -len(good)) == len(good):
                    print("--", len(good))
            if n > 10**24:
                break

    good.sort()
    iw = len(str(len(good)))
    print(e, w_max, ">", len(good))
    for i, n in enumerate(good):
        print(f"[{i:{iw}}] {n:25} {n:.2e} {n.bit_length()}", solve(n))
    return good

