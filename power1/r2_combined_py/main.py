#! /usr/bin/env pypy

def solve_1(n: int):
    """
    e = 1 の解を列挙する
    試し割り法により Θ(n^{1/2}) 時間
    """
    ans = []
    n2 = n * 2
    for w in range(1, n2 + 1):
        if w * w > n2:
            break
        wl, r = divmod(n2, w)
        if r or w >= wl or (w + wl) % 2 == 0:
            continue
        l = (wl - w + 1) // 2
        ans.append((1, l, l + w - 1))
    ans.reverse()
    return ans

def solve_2(n: int):
    """
    e = 2 の解を列挙する
    尺取り法を用いて Θ(n^{1/2}) 時間
    """
    m = 1
    for i in range(n.bit_length())[::-1]:
        m2 = m + (1 << i)
        if m2 * m2 <= n:
            m = m2
    assert m * m <= n < (m + 1) * (m + 1)
    m += 2
    ans = []
    # s.t. S(2, l, r-1) <= n
    r = 1
    s = 0
    for l in range(1, m):
        while r + 1 < m and s + r * r <= n:
            s += r * r
            r += 1
        if s == n:
            ans.append((2, l, r - 1))
        s -= l * l
    return ans

def solve_e(n: int, e: int):
    """
    e = e の解を列挙する
    累積和を用いて Θ(n^{1/e}) 時間
    """
    ans = []
    # acc[i] == S(e, 0, i)
    acc = [0]
    for i in range(1, n + 1):
        if i**e > n:
            break
        acc.append(acc[-1] + i**e)
    to_l = {s: l for l, s in enumerate(acc)}
    # find (l, r) s.t. acc[r] - acc[l-1] == n
    for r in range(1, len(acc)):
        l = to_l.get(acc[r] - n, -1) + 1
        if l >= 1:
            ans.append((e, l, r))
    return ans


def main():
    n = int(input())
    ans = solve_1(n) + solve_2(n)
    for e in range(3, n.bit_length() + 2):
        ans.extend(solve_e(n, e))
    print(len(ans))
    for t in ans:
        print(*t)
main()

