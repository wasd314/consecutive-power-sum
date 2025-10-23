#! /usr/bin/env pypy

n = int(input())

# floor(n^{1/3}) + 1
m = next(i for i in range(n + 2) if i**3 > n)
assert (m - 1)**3 <= n < m**3

b = [i * i * (i + 1) * (i + 1) // 4 for i in range(m)]

import bisect
ans = []
for l in range(1, m):
    r = bisect.bisect_left(b, b[l - 1] + n)
    if r < m and b[r] - b[l - 1] == n:
        ans.append((l, r))

print(len(ans))
for l, r in ans:
    print(l, r)
