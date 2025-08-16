from fractions import Fraction as ff
import math
from functools import cache
n = 10**5

"""
find D(e) := min D s.t. W divides [D * S(e, L, L+W-1)] for all L.
Since S(e, L, L+W-1) \\equiv S(e, 0, W-1) (mod W),
D(e) = min D s.t. W divides [D * S(e, 0, W-1)].
"""

@cache
def check(e):
    acc = [0] * (n + 1)
    for i in range(n):
        acc[i + 1] = acc[i] + i**e
    # den(n / w)
    s = {ff(acc[w], w).denominator for w in range(1, n + 1)}
    l = math.lcm(*s)
    dl = ff(sum(pow(i, e, l) for i in range(l)), l).denominator
    return s, l, dl == l

n2 = 400
w_sep = 40

def show(e):
    acc = [0] * (n2 + 1)
    for i in range(n2):
        acc[i + 1] = acc[i] + i**e
    s = {ff(acc[w], w).denominator for w in range(1, n2 + 1)}
    nd = len(str(n2))
    md = len(str(max(s)))
    for w0 in range(0, n2 + 1, w_sep):
        ds = [f"{ff(acc[w], w).denominator if w else 0 :{md}}" for w in range(w0, min(w0 + w_sep, n2 + 1))]
        print(f"{w0:{nd}}:", *ds)
    print(s)
    print(math.lcm(*s))

