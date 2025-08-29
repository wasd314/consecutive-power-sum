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

def answer_cubic_with(solver):
    """`e == 3` のケースを解答する"""
    n = int(input())
    ans = solver(n, 3)
    print(len(ans))
    for sol in ans:
        print(*sol[1:])

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
    return test_miller_rabin(n, [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47])

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

class CombinedSolver:
    def __init__(self, e_start: int, *solvers):
        self.e_start = e_start
        self.solvers = solvers
        self.name = "-".join(f.__name__ for f in solvers)

    def __call__(self, n: int):
        e_start = self.e_start
        solvers = self.solvers
        ans = []
        for e, solver in enumerate(solvers, e_start):
            ans.extend(solver(n, e))
        for e in range(e_start + len(solvers), n.bit_length() + 5):
            ans.extend(solvers[-1](n, e))
        return ans

def answer_power_with(solver: CombinedSolver):
    n = int(input())
    ans = solver(n)
    print(len(ans))
    for sol in ans:
        print(*sol)

def parse_range(size: int, line: str):
    todo = set()
    for arg in line.split(" "):
        if arg == "-":
            todo.update(range(size))
        elif "-" in arg:
            l, r = map(int, arg.split("-"))
            l = max(l, 0)
            r = min(r + 1, size)
            todo.update(range(l, r))
        else:
            x = int(arg)
            if 0 <= x < size:
                todo.add(x)
    return sorted(todo)
