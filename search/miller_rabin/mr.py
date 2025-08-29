def test_miller_rabin(n: int, bases: list, verbose=False):
    nn = n - 1
    e = (nn & -nn).bit_length() - 1
    o = n >> e
    # assert n == (o << e | 1)
    if verbose:
        print(f"test {n} = {o} << {e} | 1")
    for b in bases:
        x = pow(b, o, n)
        if verbose:
            print(f"base {b} {x}")
        if x == 1 or x == 0:
            if verbose:
                print(f"\t[ ] {x}: 1 or 0")
            continue
        for i in range(e):
            y = pow(x, 2, n)
            if y == 1:
                if x == n - 1:
                    if verbose:
                        print(f"\t[{i}] {x} -> {y}: trivial")
                    break
                else:
                    # nontrivial sqrt(1) found
                    if verbose:
                        print(f"\t[{i}] {x} -> {y}: not trivial")
                    return False
            x = y
        else:
            if verbose:
                print(f"\t[{e}] {x}: != 1")
            return False
    return True

def is_prime_64bit(n: int, verbose=False):
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
        return test_miller_rabin(n, [2], verbose)
    if n < 90_80191:
        return test_miller_rabin(n, [31, 73], verbose)
    if n < 47591_23141:
        return test_miller_rabin(n, [2, 7, 61], verbose)
    if n < 112_20046_69633:
        return test_miller_rabin(n, [2, 13, 23, 16_62803], verbose)
    if n < 3_77057_95821_54547:
        return test_miller_rabin(n, [2, 8_80937, 25_70940, 6103_86380, 41307_85767], verbose)
    return test_miller_rabin(n, [2, 325, 9375, 28178, 450775, 9780504, 17952_65022], verbose)

def is_prime_80bit(n: int, verbose=False):
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
        return test_miller_rabin(n, [2], verbose)
    if n < 90_80191:
        return test_miller_rabin(n, [31, 73], verbose)
    if n < 47591_23141:
        return test_miller_rabin(n, [2, 7, 61], verbose)
    if n < 112_20046_69633:
        return test_miller_rabin(n, [2, 13, 23, 16_62803], verbose)
    if n < 3_77057_95821_54547:
        return test_miller_rabin(n, [2, 8_80937, 25_70940, 6103_86380, 41307_85767], verbose)
    if n < 18446_74407_37095_51616:
        return test_miller_rabin(n, [2, 325, 9375, 28178, 450775, 9780504, 17952_65022], verbose)
    if n < 3186_65857_83403_11511_67461:
        return test_miller_rabin(n, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37], verbose)
    if n < 33170_44064_67988_73859_61981:
        return test_miller_rabin(n, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41], verbose)
    assert False

def is_prime_2_64(n: int, verbose=False):
    if n < 2:
        return False
    return test_miller_rabin(n, [2, 325, 9375, 28178, 450775, 9780504, 17952_65022], verbose)

from math import gcd

def factorize(n: int, is_prime=is_prime_64bit):
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

# for n in range(1, 10**6):
#     p1 = is_prime(n)
#     p2 = is_prime_2_64(n)
#     if p1 != p2:
#         print(n, p1, p2)
#         break

def search():
    """
    is_prime_64bit が True を返す合成数を探す

    `./carm_2sp` は

    - https://www-labs.iro.umontreal.ca/~goutier/OEIS/A055553

    から得られる `carm10e22` （10^22 以下の Carmichael number）のうち，"strong pseudoprime to base 2" を抜き出したもの
    """
    ns = []
    with open("./carm_2sp") as f:
        for s in f:
            n = int(s.rstrip("\n"))
            if is_prime_2_64(n):
                print(f"{n:25} ({n:e}) : {is_prime_64bit(n)=}, {is_prime_80bit(n)=}")
                ns.append(n)
    print(ns)
search()

