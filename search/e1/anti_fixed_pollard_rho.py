"""
Pollard's rho において乱数生成器 f_c := x |-> x^2 + c を固定する実装を無限ループにすべく，
各 c に対して (f_c^i)_i の巡回周期が短い合成数を探す
see: https://lpha-z.hatenablog.com/entry/2023/01/15/231500
"""

class UnionFind:
    def __init__(self, n: int):
        self.parent = [-1 for i in range(n)]
        self.rank = [1 for i in range(n)]
        # self.member = [set([i]) for i in range(n)]
        self.leaders = set(range(n))
        """代表元の集合
        """

    def find(self, x: int) -> int:
        """x が属する集合の代表元

        Args:
            x (int): 0-indexed

        Returns:
            int: 代表元
        """
        if self.parent[x] < 0:
            return x
        self.parent[x] = self.find(self.parent[x])
        return self.parent[x]

    def unite(self, x: int, y: int) -> int:
        """x, y が属する集合を合併

        Args:
            x (int): 0-indexed
            y (int): 0-indexed

        Returns:
            int: 減少した集合数
        """
        x = self.find(x)
        y = self.find(y)
        if x == y:
            return 0
        if self.rank[x] < self.rank[y]:
            x, y = y, x
        if self.rank[x] == self.rank[y]:
            self.rank[x] += 1
        # x を新しい親に
        self.parent[x] += self.parent[y]
        self.parent[y] = x
        self.leaders.discard(y)
        # self.member[x] |= self.member[y]
        return 1

    def same(self, x: int, y: int) -> bool:
        """x と y が同じ集合に属するか

        Args:
            x (int): 0-indexed
            y (int): 0-indexed

        Returns:
            bool: 同じ集合に属するか
        """
        return self.find(x) == self.find(y)

    def size(self, x: int) -> int:
        """x が属する集合の要素数

        Args:
            x (int): 0-indexed

        Returns:
            int: 要素数
        """
        x = self.find(x)
        return -(self.parent[x])

    def group_num(self) -> int:
        """全体の集合数

        Returns:
            int: 集合の数
        """
        return len(self.leaders)

    def all_members(self):
        """全集合の列挙

        Returns:
            dict[int, set[int]]: 代表元 -> 要素列
        """
        to_mem = {l: {l} for l in self.leaders}
        for v in range(len(self.parent)):
            to_mem[self.find(v)].add(v)
        return to_mem


import heapq
import math
import random
from functools import cache
from collections import defaultdict, deque
THRESHOLD = 4000

def brent(f, x0, stop=True, proj=None):
    """
    列 (f^i(x0))_i の周期を返す
    """
    if proj is None:
        proj = lambda x: x
    x = x0
    y = f(x)
    i2 = 1
    while not stop or i2 < THRESHOLD * 4:
        # y = x_i2
        x = y
        # x = x_i2
        for j in range(1, i2 + 1):
            y = f(y)
            # y = x_(i2 + j)
            if proj(x) == proj(y):
                return i2, j
        # y = x_(i2 + i2)
        i2 *= 2
    return i2, i2

@cache
def f2(c: int, n: int):
    def f(x: int):
        return (x * x + c) % n
    return f

def cycle1(c, n, x0):
    return brent(f2(c, n), x0)

def cycle2(c, n, k=10):
    mx = 1
    lm = 1
    for x0 in random.sample(range(n), min(n, k)):
        l = cycle1(c, n, x0)
        mx = max(mx, l[1])
        lm = math.lcm(lm, l[1])
        if lm > THRESHOLD:
            lm = THRESHOLD + 1
            break
    return mx, lm

@cache
def cycle3(c, n):
    mx = 1
    lm = 1
    for k in [1, 5, 20, 200, 1000]:
        mx_, lm_ = cycle2(c, n, k)
        mx = max(mx, mx_)
        lm = math.lcm(lm, lm_)
        if lm > THRESHOLD:
            lm = THRESHOLD + 1
            break
    return mx, lm

@cache
def cycle_uf(c, n):
    f = f2(c, n)
    uf = UnionFind(n)
    for x in range(n):
        uf.unite(x, f(x))
    return uf

def cycle_periods(c, n):
    uf = cycle_uf(c, n)
    return [cycle1(c, n, x0)[1] for x0 in uf.leaders]
def cycle_sizes(c, n):
    uf = cycle_uf(c, n)
    return [(x0, cycle1(c, n, x0)[1], uf.size(x0)) for x0 in uf.leaders]

@cache
def cycle4(c, n):
    """
    f の周期を真面目に求める
    Ω(n) 時間かかる
    """
    periods = cycle_periods(c, n)
    return max(periods), math.lcm(*periods)


class FinFn:
    def __init__(self, c: int, n: int) -> None:
        self.c = c
        self.n = n
        f = f2(c, n)
        dou = [[f(i) for i in range(n)]]
        self.doubling = dou
        self.cache_cycle_pre = 0, 0

    def extend(self, e: int):
        be = e.bit_length()
        dou = self.doubling
        while len(dou) < be:
            do = dou[-1]
            ndo = [do[j] for j in do]
            dou.append(ndo)

    def pow(self, e: int):
        n = self.n
        self.extend(e)
        g = list(range(n))
        for i, do in enumerate(self.doubling):
            if (e >> i) == 0:
                break
            if e >> i & 1:
                ng = [do[j] for j in g]
                g = ng
        return g

    @cache
    def cycle_pre(self):
        """
        f の前周期と周期を求める
        周期は cycle4 の出力に基づく
        """
        if self.cache_cycle_pre != (0, 0):
            return self.cache_cycle_pre
        period = cycle4(self.c, self.n)[1]
        f_loop = self.pow(period)

        def pred(m):
            # f^m == f^(m + period) ?
            return all(j == f_loop[j] for j in self.pow(m))

        ng = -1
        ok = 1
        while not pred(ok):
            ok *= 2
        while abs(ok - ng) > 1:
            mi = (ok + ng) // 2
            if pred(mi):
                ok = mi
            else:
                ng = mi
        self.cache_cycle_pre = ok, period
        return self.cache_cycle_pre

    @cache
    def each_cycle_pre(self):
        """
        各初期値からの f の前周期と周期を求める
        """
        f = self.doubling[0]
        fn = self.pow(1 << self.n.bit_length() + 1)
        ans = [(-1, -1)] * self.n
        uf = cycle_uf(self.c, self.n)
        for x in uf.leaders:
            x = fn[x]
            path = [x]
            while f[path[-1]] != x:
                path.append(f[path[-1]])
            l = len(path)
            for y in path:
                ans[y] = 0, l
        q = deque(set(fn))
        f_inv = [[] for _ in range(self.n)]
        for i, fi in enumerate(f):
            f_inv[fi].append(i)
        while q:
            v = q.popleft()
            nd = ans[v][0] + 1, ans[v][1]
            for nv in f_inv[v]:
                if ans[nv] == (-1, -1):
                    ans[nv] = nd
                    q.append(nv)
        return ans


@cache
def fin_fn(c, n):
    return FinFn(c, n)

@cache
def cycle_pre(c, n, stop=False):
    """
    f の周期を愚直に求める
    Ω(n) 時間かかる
    """
    f = f2(c, n)
    return brent(lambda l: [f(e) for e in l], list(range(n)), stop=stop)

def composed_cycle_pre(c, ns, show=True, pick2=True):
    ns = list(ns)
    if show:
        print(c, ns)
    cp0 = [(fin_fn(c, n).cycle_pre(), n) for n in ns]
    cp = cp0[:]
    if pick2:
        cp_ = cp0[:]
        heapq.heapify(cp_)
        cp = [heapq.heappop(cp_) for _ in range(min(2, len(cp0)))]
    m = max(t[0][0] for t in cp)
    l = math.lcm(*[t[0][1] for t in cp])
    if show:
        for t, n in cp0:
            print(" *"[(t, n) in cp], n, t)
        print(f"\t^{m} == ^{m + l}")
    return m, l, m + l


import sympy as sp
sp.sieve.extend(10**7)

def search(c: int, n_min: int, n_max: int, w: int, pre: defaultdict):
    print(f"{c=}, {THRESHOLD=}")
    for n_r in range(n_max, n_min, -w):
        n_l = max(n_r - w, n_min)
        print(">", (n_l, n_r), len(pre))
        p: int
        for p in reversed(list(sp.primerange(n_l, n_r))):
            m, l = cycle3(c, p)
            if l < THRESHOLD:
                m, l = cycle4(c, p)
            if l < THRESHOLD and m == l:
                uf = cycle_uf(c, p)
                if uf.group_num() > 1:
                    continue
                # if len(set(cycle_periods(c, p))) > 1:
                #     continue
                pre[l].append(p)
                if len(pre[l]) > 1:
                    print(p, composed_cycle_pre(c, pre[l], False), pre[l])
c = 1
w = 10**5
n_mx = 10**6
pre = defaultdict(list)

