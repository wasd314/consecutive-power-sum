#! /usr/bin/env pypy
from collections import Counter
from math import gcd

def power_sum(e: int, l: int, r: int | None = None) -> int:
    """e-th power sum of [l, r) or [0, l)"""
    if r is not None:
        if 1 <= e <= 5:
            return power_sum(e, r) - power_sum(e, l)
        return sum(i**e for i in range(l, r))
    elif e == 1:
        return l * (l - 1) // 2
    elif e == 2:
        return l * (l - 1) * (2 * l - 1) // 6
    elif e == 3:
        return l * (l - 1) // 2 * l * (l - 1) // 2
    elif e == 4:
        return l * (l - 1) * (2 * l - 1) * (3 * l * l - 3 * l - 1) // 30
    elif e == 5:
        return (l - 1) * l // 2 * (l - 1) * l // 2 * (2 * (l - 1) * l - 1) // 3
    else:
        return 0

def min_true(l: int, r: int, pred):
    """min i s.t. l <= i < r and pred(i)"""
    assert l < r
    if pred(l):
        return l
    while abs(l - r) > 1:
        m = (l + r) // 2
        if pred(m):
            r = m
        else:
            l = m
    return r

def test_miller_rabin(n: int, bases: list):
    nn = n - 1
    e = (nn & -nn).bit_length() - 1
    o = n >> e
    # assert n == (o << e | 1)
    for b in bases:
        x = pow(b, o, n)
        if x == 1:
            continue
        for _ in range(e):
            y = pow(x, 2, n)
            if y == 1:
                if x == n - 1:
                    break
                else:
                    # nontrivial sqrt(1) found
                    return False
            x = y
        else:
            return False
    return True

def is_prime(n: int):
    if n < 2:
        return False
    for p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        if n == p:
            return True
        if n % p == 0:
            return False
    if n < 41**2:
        return True
    if n < 2047:
        return test_miller_rabin(n, [2])
    if n < 90_80191:
        return test_miller_rabin(n, [31, 73])
    if n < 47591_23141:
        return test_miller_rabin(n, [2, 7, 61])
    if n < 112_20046_69633:
        return test_miller_rabin(n, [2, 13, 23, 16_62803])
    if n < 3_77057_95821_54547:
        return test_miller_rabin(n, [2, 8_80937, 25_70940, 6103_86380, 41307_85767])
    if n < 18446_74407_37095_51616:
        return test_miller_rabin(n, [2, 325, 9375, 28178, 450775, 9780504, 17952_65022])
    if n < 3186_65857_83403_11511_67461:
        return test_miller_rabin(n, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37])
    if n < 33170_44064_67988_73859_61981:
        return test_miller_rabin(n, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41])
    assert False

def factorize(n: int):
    assert n >= 1
    if n == 1:
        return []
    if is_prime(n):
        return [n]
    ans = []
    for p in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        while n % p == 0:
            n //= p
            ans.append(p)

    def dfs(nn: int):
        if nn == 1:
            return
        if is_prime(nn):
            ans.append(nn)
            return

        factor_round = 1 << nn.bit_length() // 8

        def find_factor():
            c = 0

            def f(x):
                return (x * x + c) % nn

            while True:
                c += 1
                x, y = c, c
                d = 1
                checkpoint = x, y
                while d == 1:
                    combined = 1
                    for _ in range(factor_round):
                        # Floyd's
                        x, y = f(x), f(f(y))
                        combined = combined * abs(x - y) % nn
                    d = gcd(combined, nn)
                    if d == 1:
                        # この round では見つからなかった
                        checkpoint = x, y
                    elif d != nn:
                        # 非自明な約数
                        return d
                    # else: d == nn: maybe found, break

                # 1つずつ進める
                x, y = checkpoint
                d = 1
                while d == 1:
                    x, y = f(x), f(f(y))
                    d = gcd(abs(x - y), nn)
                    if d != 1 and d != nn:
                        return d
        d = find_factor()
        dfs(d)
        dfs(nn // d)
    dfs(n)
    return ans

def list_divisors(pe: dict[int, int], pred):
    if not pred(1):
        return []
    ds = [1]
    for p, e in pe.items():
        for i in range(len(ds)):
            d = ds[i]
            for _ in range(e):
                d *= p
                if not pred(d):
                    break
                ds.append(d)
    return ds


def r13(n: int, e: int):
    ans = []
    r = n
    e2 = 1
    while r % 2 == 0:
        r //= 2
        e2 += 1
    # (r << e2) == 2 * n
    pe_r = Counter(factorize(r))
    for a in list_divisors(pe_r, lambda a: True):
        b = (r // a) << e2
        if a > b:
            a, b = b, a
        w, w2 = a, b
        l = (w2 - w + 1) // 2
        ans.append((1, l, l + w - 1))
    ans.sort()
    return ans

enough_denom = [1, 2, 6, 2, 30, 2, 42, 2, 30, 2, 66, 2, 2730, 2, 6, 2, 510, 2, 798, 2, 330, 2, 138, 2, 2730, 2, 6, 2, 870, 2, 14322, 2, 510, 2, 6, 2, 1919190, 2, 6, 2, 13530, 2, 1806, 2, 690, 2, 282, 2, 46410, 2, 66, 2, 1590, 2, 798, 2, 870, 2, 354, 2, 56786730, 2, 6, 2, 510, 2, 64722, 2, 30, 2, 4686, 2, 140100870, 2, 6, 2, 30, 2, 3318, 2, 230010, 2, 498, 2, 3404310, 2, 6, 2, 61410, 2, 272118, 2, 1410, 2, 6, 2, 4501770, 2, 6, 2]

def re1_bs_fac(n: int, e: int, from_prev: bool):
    pe_dn = Counter(factorize(enough_denom[e] * n))
    divisors = list_divisors(pe_dn, lambda w: power_sum(e, 1, w + 1) <= n)
    if from_prev:
        divisors.sort()
    ans = []
    prev_l = n
    for w in divisors:
        def pred(l):
            return power_sum(e, l, l + w) >= n
        if from_prev and w != 1:
            dl = 1
            while dl < prev_l and pred(prev_l - dl):
                dl <<= 1
            l = min_true(max(0, prev_l - dl), prev_l + 1, pred)
            prev_l = l
        else:
            r = 1
            while not pred(r):
                r <<= 1
            l = min_true(0, r, pred)
        if power_sum(e, l, l + w) == n:
            ans.append((e, l, l + w - 1))
    ans.sort()
    return ans

def re0_two_pointer(n: int, e: int):
    pows = []
    for i in range(n + 1):
        if i**e <= n:
            pows.append(i**e)
        else:
            break
    # pows[i] = i**e

    c = len(pows)
    ans = []
    r = 1
    current_sum = 0
    for l in range(1, c):
        while r < c and current_sum + pows[r] <= n:
            current_sum += pows[r]
            r += 1
        # (r >= c or) current_sum <= n < current_sum + pows[r]
        if current_sum == n:
            ans.append((e, l, r - 1))
        if l < r:
            current_sum -= pows[l]
    ans.sort()
    return ans

def solve(n: int):
    ans = r13(n, 1)
    for e in range(2, n.bit_length() + 5):
        if e <= 3:
            ans.extend(re1_bs_fac(n, e, True))
        else:
            ans.extend(re0_two_pointer(n, e))
    return ans

n = int(input())
ans = solve(n)
print(len(ans))
for t in ans:
    print(*t)
