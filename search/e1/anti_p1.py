"""
p+1 法, p-1 法素因数分解の対策として，10^12 弱で p-1, p+1 が大きい素因数を含むような p を探す
"""

import heapq
from functools import cache

def sieves(nn: int):
    is_prime = [True] * nn
    lpf = [-1] * nn
    is_prime[0] = is_prime[1] = False
    lpf[1] = 1
    for p in range(2, nn):
        if not is_prime[p]:
            continue
        lpf[p] = p
        for q in range(p * 2, nn, p):
            is_prime[q] = False
            if lpf[q] == -1:
                lpf[q] = p
    primes = [p for p in range(2, nn) if is_prime[p]]
    return is_prime, lpf, primes

w = 10 ** 7
_, _, primes0 = sieves(w)

@cache
def segment_sieve(l, r):
    r = max(l, r)
    w2 = r - l
    is_prime = [True] * w2
    pf = [[] for _ in range(w2)]
    rem = list(range(l, r))
    for i in range(2):
        if l <= i < r:
            is_prime[i - l] = False
            rem[i] = 1
    # w 以下の素数で割る
    for p in primes0:
        pl = max(l + (-l) % p, p * 2)
        for q in range(pl, r, p):
            is_prime[q - l] = False
            while rem[q - l] % p == 0:
                rem[q - l] //= p
                pf[q - l].append(p)
    # 残りは素数
    for i in range(l, r):
        if rem[i - l] > 1:
            pf[i - l].append(rem[i - l])
    primes1 = [p for p in range(l, r) if is_prime[p - l]]
    return is_prime, primes1, pf

smooth_limit = 10**6
SHOW_NICE = 10

def find_nice_prime(w, r, m=1):
    """
    p is nice :<=> p +/- 1 が (3-smooth) * prime
    [r-w, r) の範囲の nice prime を探す
    mod 12 ごとに表示
    """
    l = r - w
    if l % 2:
        l -= 1
    if r % 2 == 0:
        r += 1
    print(f"{l:_}, {r:_}")
    _, primes1, pf = segment_sieve(l, r)

    def score(p):
        return min(max(pf[p - 1 - l]), max(pf[p + 1 - l]))

    primes_by_rem = {rem: [] for rem in [1, 5, 7, 11]}
    for p in primes1:
        q = primes_by_rem[p % 12]
        heapq.heappush(q, (score(p), p))
        while len(q) > SHOW_NICE:
            heapq.heappop(q)
    for rem, pso in sorted(primes_by_rem.items()):
        print(f"{rem=}")
        pso.sort(reverse=True)
        for i, (s, p) in enumerate(pso):
            print(f"{i:2} {p:17_} ({m * p}) : {s:_} ; {max(pf[p - 1 - l]):_}, {max(pf[p + 1 - l]):_}")

def find_nice_prime2(w, max_n, p):
    """
    p の相方の nice prime を探す
    積が max_n を超えない範囲で
    """
    find_nice_prime(w, max_n // p, p)


w = 10**6
n = 10**12

def pd(d: float):
    """p s.t. p * q == n * n and q - p = n * d"""
    return int(n * 2 / (d + (d * d + 4)**.5))
def qd(d: float):
    """q s.t. p * q == n * n and q - p = n * d"""
    return int(n * (d + (d * d + 4)**.5) / 2)


