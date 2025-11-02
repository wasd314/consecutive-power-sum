import sympy as sp
sp.sieve.extend(10**7)

import anti_fixed_pollard_rho as anti
import random
from functools import cache
from collections import defaultdict

OO = anti.THRESHOLD + 1

def single_cycle2(c, n, k=10):
    """
    k 個乱択した初期値からの周期を返す
    周期が2種類出てきたら `None`
    """
    lm = 0
    for x0 in random.sample(range(n), min(n, k)):
        l = anti.cycle1(c, n, x0)[1]
        if l >= anti.THRESHOLD or (lm != 0 and l != lm):
            return
        lm = l
    return lm

def single_cycle3(c, n):
    """
    single_cycle2 を適当な k で呼ぶ
    """
    lm = 0
    for k in [1, 5, 20, 200, 1000]:
        l = single_cycle2(c, n, k)
        if l is None or (lm != 0 and l != lm):
            return
        lm = l
    return lm

@cache
def single_cycle_pre(c, n):
    # cf. abc357e
    f = anti.f2(c, n)
    g = [f(i) for i in range(n)]
    mu = [-1] * n
    lam = [-1] * n
    state = [-1] * n
    for s in range(n):
        if state[s] >= 0:
            continue
        v = s
        state[v] = s
        i = 1
        while state[g[v]] == -1:
            v = g[v]
            i += 1
            state[v] = s
        if state[g[v]] == s:
            r = g[v]
            l = 1
            v = r
            mu[v] = 0
            while g[v] != r:
                v = g[v]
                mu[v] = 0
                l += 1
            v = r
            lam[v] = l
            while g[v] != r:
                v = g[v]
                lam[v] = l
            i -= l
        m = mu[g[v]]
        l = lam[g[v]]
        v = s
        for j in range(i)[::-1]:
            mu[v] = m + j + 1
            lam[v] = l
            v = g[v]
    if len(set(lam)) > 1:
        return OO, OO
    return max(mu), lam[0]

def is_good1(c, n):
    lam = single_cycle3(c, n)
    if lam is None:
        return False, 0
    if set(anti.cycle_periods(c, n)) != {lam}:
        return False, 0
    ff = anti.fin_fn(c, n)
    pow_l = ff.pow(lam)
    return all(pow_l[x] == x for x in set(pow_l)), lam
    # m, l = cycle3(c, p)
    # if l < THRESHOLD:
    #     m, l = cycle4(c, p)
    # if l < THRESHOLD and m == l:
    #     uf = cycle_uf(c, p)
    #     if uf.group_num() > 1:
    #         continue
    #     # if len(set(cycle_periods(c, p))) > 1:
    #     #     continue
    #     pre[l].append(p)
    #     if len(pre[l]) > 1:
    #         print(p, composed_cycle_pre(c, pre[l], False), pre[l])

def search(c, n_min, n_max, w, pre: defaultdict[int, list]):
    print(f"{c=}, {anti.THRESHOLD=}, ({n_min} -- {n_max})")
    for i, n_l in enumerate(range(n_min, n_max, w)):
        n_r = min(n_l + w, n_max)
        print(">", (n_l, n_r), len(pre), flush=True)
        if i % 10 == 0:
            print(f"{pre=}", flush=True)
        p: int
        for p in sp.primerange(n_l, n_r):
            ok, l = is_good1(c, p)
            if not ok:
                continue
            pre[l].append(p)
            if len(pre[l]) > 1:
                print(p, anti.composed_cycle_pre(c, [p], False), anti.composed_cycle_pre(c, pre[l], False), pre[l], flush=True)
            else:
                print(p, anti.composed_cycle_pre(c, [p], False), [])

w = 10**5
pres = defaultdict(lambda: defaultdict(list))
for i in range(5):
    for c in [1, 3, -1, 2, 5, 4]:
        search(c, w * 10 * i, w * 10 * (i + 1), w, pres[c])
        print()
