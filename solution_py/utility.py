def power_sum(e: int, l: int, r: int = None) -> int:
    """e-th power sum of [l, r) or [0, l)"""
    if r is not None:
        return power_sum(e, r) - power_sum(e, l)
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

def answer_power_with(args: tuple):
    """`e >= args[0]` のケースを `args[1:]` で解答する"""
    n = int(input())
    ans = []
    e_start, *solvers = args
    for e, solver in enumerate(solvers, e_start):
        ans.extend(solver(n, e))
    for e in range(len(solvers) + e_start, 65):
        ans.extend(solvers[-1](n, e))
    print(len(ans))
    for sol in ans:
        print(*sol)

