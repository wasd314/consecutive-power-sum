#! /usr/bin/env pypy

def solve_1(n: int):
    """
    e = 1 の解を列挙する
    試し割り法により Θ(n^{1/2}) 時間
    """
    ans = []
    n2 = n * 2
    for w in range(1, n2):
        if w * w > n2:
            break
        wl, r = divmod(n2, w)
        if r or w >= wl or (w + wl) % 2 == 0:
            continue
        l = (wl - w + 1) // 2
        ans.append((1, l, l + w - 1))
    ans.reverse()
    return ans

def solve_e(n: int, e: int):
    """
    e = e の解を列挙する
    尺取り法を用いて Θ(n^{1/e}) 時間
    """
    # floor(n^{1/e}) + 1
    m = next(i for i in range(n + 2) if i**e > n)
    assert (m - 1)**e <= n < m**e

    ans = []
    # S(e, l, r-1) == s になるようにする
    r = 1
    s = 0
    for l in range(1, m):
        while s + r**e <= n:
            s += r**e
            r += 1
        if s == n:
            ans.append((e, l, r - 1))
        s -= l**e
    return ans

n = int(input())
assert 2 <= n <= 10**12

ans = solve_1(n)
for e in range(2, n.bit_length()):
    ans.extend(solve_e(n, e))

print(len(ans))
for t in ans:
    print(*t)
