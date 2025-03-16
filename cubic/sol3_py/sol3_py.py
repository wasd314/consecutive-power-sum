#! /usr/bin/env python

def cubic_sum(l: int, r: int = None):
    # cubic sum of [i, r) or [0, l)
    if r is not None:
        return cubic_sum(r) - cubic_sum(l)
    return l * (l - 1) // 2 * l * (l - 1) // 2

import bisect as bs

def solve(n: int):
    ans = []
    for w in range(1, n + 1):
        if cubic_sum(1, w + 1) > n:
            break
        if 4 * n % w:
            continue
        r = 1
        while cubic_sum(r, r + w) <= n:
            r *= 2
        l = bs.bisect_left(range(r + 1), n, key=lambda l: cubic_sum(l, l + w), lo=1)
        if cubic_sum(l, l + w) == n:
            ans.append((l, l + w - 1))
    ans.sort()
    return ans

if __name__ == "__main__":
    n = int(input())
    ans = solve(n)
    print(len(ans))
    for t in ans:
        print(*t)
