#! /usr/bin/env python

def power_sum(e: int, l: int, r: int = None):
    # cubic sum of [i, r) or [0, l)
    if r is not None:
        return power_sum(e, r) - power_sum(e, l)
    if e == 2:
        return l * (l - 1) * (2 * l - 1) // 6
    if e == 3:
        return l * (l - 1) // 2 * l * (l - 1) // 2
    if e == 4:
        return l * (l - 1) * (2 * l - 1) * (3 * l * l - 3 * l - 1) // 30


import bisect as bs

def solve_3(n: int):
    ans = []
    for w in range(1, n + 1):
        if power_sum(3, 1, w + 1) > n:
            break
        if 4 * n % w:
            continue
        r = 1
        while power_sum(3, r, r + w) <= n:
            r *= 2
        l = bs.bisect_left(range(r + 1), n, key=lambda l: power_sum(3, l, l + w), lo=1)
        if power_sum(3, l, l + w) == n:
            ans.append((3, l, l + w - 1))
    return ans

def solve_e(n: int, e: int):
    if n < 1 + 2**e:
        return []
    power = []
    for i in range(n + 1):
        if i**e <= n:
            power.append(i**e)
        else:
            break
    # power[i] = i**e

    c = len(power)
    acc = [0] * (c + 1)
    for i in range(c):
        acc[i + 1] = acc[i] + power[i]
    # acc[i] = sum(j**e for j < i)
    # acc[i + 1] = sum(j**e for j <= i)

    right = {acc[i + 1]: i for i in range(1, c)}
    ans = []
    for l in range(1, c + 1):
        r = right.get(acc[l] + n, -1)
        if r != -1:
            ans.append((e, l, r))
    return ans

def solve(n: int):
    ans = []
    ans.extend(solve_3(n))
    for e in range(4, 70):
        ans.extend(solve_e(n, e))
    ans.sort()
    return ans

if __name__ == "__main__":
    n = int(input())
    ans = solve(n)
    print(len(ans))
    for t in ans:
        print(*t)
