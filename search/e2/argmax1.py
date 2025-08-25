import sympy as sp
from datetime import datetime
from functools import cache
from bisect import bisect_right
# argmax d(6N, (3N)^{1/3}) を探す

primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,]
primorials = [1] + primes
for i in range(len(primes)):
    primorials[i + 1] *= primorials[i]

def list_divisors(pe: dict[int, int], pred):
    if not pred(1):
        return []
    ds = [1]
    for p, e in pe.items():
        for i in range(len(ds)):
            d = ds[i]
            for _ in range(1, e + 1):
                d *= p
                if not pred(d):
                    break
                ds.append(d)
    return ds

def acc2(l: int, r: int):
    """sum(i**2 for i in range(l, r))"""
    l, r = l - 1, r - 1
    sr = r * (r + 1) * (2 * r + 1) // 6
    sl = l * (l + 1) * (2 * l + 1) // 6
    return sr - sl

@cache
def factorize(n: int):
    pe = {}
    for p in primes:
        if n == 1:
            break
        if n % p:
            continue
        e = 0
        while n % p == 0:
            n //= p
            e += 1
        pe[p] = e
    if n > 1:
        for p, e in sp.factorint(n).items():
            pe[p] = e
    return pe

color_dic = {"black": "\033[30m", "red": "\033[31m", "green": "\033[32m", "yellow": "\033[33m", "blue": "\033[34m", "end": "\033[0m"}
def wrap_color(text, color="red"):
    return color_dic[color] + text + color_dic["end"]

def factorize_str(n: int):
    pe = factorize(n)
    es = [pe.get(p, 0) for p in primes]
    return "".join("_" if e == 0 else str(e) if e < 10 else wrap_color(chr(ord("a") + e - 10), "red") for e in es)

@cache
def count_divisors(n6: int, n6_max: int):
    """actual d(n6, w_max(n)), upper bound d(n6, w_max(n_max))"""
    pe = factorize(n6)
    d_ac = len(list_divisors(pe, lambda w: acc2(1, w + 1) * 6 <= n6))
    d_ub = len(list_divisors(pe, lambda w: acc2(1, w + 1) * 6 <= n6_max))
    return d_ac, d_ub

@cache
def count_smooth(ip, n):
    """n 以下の primes[ip]-smooth な正整数の個数 (ABC300-G)"""
    p = primes[ip]
    l1 = [1]
    l2 = [1]
    for pi in primes:
        if pi > p:
            break
        if len(l1) > len(l2):
            l1, l2 = l2, l1
        pp = [1]
        while pp[-1] <= n:
            pp.append(pp[-1] * pi)
        l1.sort()
        l1 = [x * pe for pe in pp for x in l1[:bisect_right(l1, n // pe)]]
    l1.sort()
    l2.sort()
    ans = 0
    for x in l2:
        i = bisect_right(l1, n // x)
        ans += i
    return ans


ub_memo = {}

def upper_bound(n_max: int):
    """
    primorial の積に限って探索する
    """
    n6_max = n_max * 6
    fw = len(f"{n6_max:_}")
    cand = {}

    def dfs(ip: int, n6: int):
        if n6 > n6_max:
            return
        if ip <= -1:
            cand[n6] = f"{n6:{fw}_}", factorize_str(n6), *count_divisors(n6, n6_max)
            if len(cand) % 5000 == 0:
                print(f"[{datetime.now()}]", len(cand))
            return
        pm = primorials[ip + 1]
        while n6 <= n6_max:
            dfs(ip - 1, n6)
            n6 *= pm

    print(f"[{datetime.now()}] start n6_max = {n6_max:_}")
    if n_max in ub_memo:
        cand, cand_ac, to_rank, cand_ub = ub_memo[n_max]
    else:
        dfs(len(primes) - 1, 1)
        cand_ac = sorted(cand, key=lambda n: (-cand[n][-2], -cand[n][-1]))
        to_rank = {n: i for i, n in enumerate(cand_ac)}
        cand_ub = sorted(cand, key=lambda n: (-cand[n][-1], -cand[n][-2]))
        ub_memo[n_max] = cand, cand_ac, to_rank, cand_ub

    print(f"[{datetime.now()}] {len(cand):_} cand found")
    for i in range(200):
        n_ac = cand_ac[i]
        n_ub = cand_ub[i]
        print(to_rank[n_ac], *cand[n_ac], "|", to_rank[n_ub], *cand[n_ub], sep="\t")
    print("...")
    for i in range(20)[::-1]:
        n_ac = cand_ac[~i]
        n_ub = cand_ub[~i]
        print(to_rank[n_ac], *cand[n_ac], "|", to_rank[n_ub], *cand[n_ub], sep="\t")

