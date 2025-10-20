#! /usr/bin/env pypy

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
    return test_miller_rabin(n, [2, 325, 9375, 28178, 450775, 9780504, 17952_65022])

from math import gcd


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
            x0 = 0

            def f(x):
                return (x * x + 1) % nn

            while True:
                x0 += 1
                x, y = x0, f(x0)
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


def solve():
    n = int(input())
    factorize(n)

if __name__ == "__main__":
    solve()
