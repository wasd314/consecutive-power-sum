#! /usr/bin/env pypy

n = int(input())

# floor(n^{1/3}) + 1
m = next(i for i in range(n + 2) if i**3 > n)
assert (m - 1)**3 <= n < m**3

b = [i * i * (i + 1) * (i + 1) // 4 for i in range(m)]
to_i = {bi: i for i, bi in enumerate(b)}

ans = []
for l in range(1, m):
    r = to_i.get(b[l - 1] + n)
    if r is not None:
        ans.append((l, r))

print(len(ans))
for l, r in ans:
    print(l, r)
