#! /usr/bin/env pypy

n = int(input())
assert 1 <= n <= 10**18

# floor(n^{1/3}) + 1
m = next(i for i in range(n + 2) if i**3 > n)
assert (m - 1)**3 <= n < m**3

ans = []
# sum(i**3 for i in range(l, r)) == s になるようにする
r = 1
s = 0
for l in range(1, m):
    while s + r**3 <= n:
        s += r**3
        r += 1
    if s == n:
        ans.append((l, r - 1))
    s -= l**3

print(len(ans))
for l, r in ans:
    print(l, r)
