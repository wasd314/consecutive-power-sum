def power_sum(e: int, l: int, r: int = None) -> int:
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
        for e in range(len(solvers) + e_start, 65):
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
