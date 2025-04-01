from utility import power_sum
import bisect as bs

def dummy(*args):
    pass

def r30_dict(n: int, _):
    cube = []
    for i in range(n + 1):
        if i * i * i <= n:
            cube.append(i * i * i)
        else:
            break
    # cube[i] = i**3

    c = len(cube)
    acc = [0] * (c + 1)
    for i in range(c):
        acc[i + 1] = acc[i] + cube[i]
    # acc[i] = sum(j**3 for j < i)
    # acc[i + 1] = sum(j**3 for j <= i)

    right = {acc[i + 1]: i for i in range(1, c)}
    ans = []
    for l in range(1, c + 1):
        r = right.get(acc[l] + n, -1)
        if r != -1:
            ans.append((3, l, r))
    ans.sort()
    return ans

def r30_acc_bs(n: int, _):
    cube = []
    for i in range(n + 1):
        if i * i * i <= n:
            cube.append(i * i * i)
        else:
            break
    # cube[i] = i**3

    c = len(cube)
    acc = [0] * (c + 1)
    for i in range(c):
        acc[i + 1] = acc[i] + cube[i]
    # acc[i] = sum(j**3 for j < i)
    # acc[i + 1] = sum(j**3 for j <= i)

    ans = []
    for l in range(1, c + 1):
        r = bs.bisect_left(acc, acc[l] + n)
        if r == c + 1:
            break
        if acc[r] - acc[l] == n:
            ans.append((3, l, r - 1))
    ans.sort()
    return ans

def r30_two_pointer(n: int, _):
    cube = []
    for i in range(n + 1):
        if i * i * i <= n:
            cube.append(i * i * i)
        else:
            break
    # cube[i] = i**3

    c = len(cube)
    ans = []
    r = 1
    current_sum = 0
    for l in range(1, c):
        while r < c and current_sum + cube[r] <= n:
            current_sum += cube[r]
            r += 1
        # (r >= c or) current_sum <= n < current_sum + cube[r]
        if current_sum == n:
            ans.append((3, l, r - 1))
        if l < r:
            current_sum -= cube[l]
    ans.sort()
    return ans

def r31_bs(only_div: bool, from_prev: bool):
    def inner(n: int, _):
        ans = []
        prev_l = n
        for w in range(1, n + 1):
            if power_sum(3, 1, w + 1) > n:
                break
            if only_div and 4 * n % w:
                continue
            if from_prev:
                dl = 1
                while dl < prev_l and power_sum(3, prev_l - dl, prev_l - dl + w) >= n:
                    dl <<= 1
                l = bs.bisect_left(range(prev_l + 1), n, key=lambda l: power_sum(3, l, l + w), lo=max(0, prev_l - dl))
                prev_l = l
            else:
                r = 1
                while power_sum(3, r, r + w) <= n:
                    r <<= 1
                l = bs.bisect_left(range(r + 1), n, key=lambda l: power_sum(3, l, l + w), lo=1)
            if power_sum(3, l, l + w) == n:
                ans.append((3, l, l + w - 1))
        ans.reverse()
        ans.sort()
        return ans
    inner.__name__ = f"r31_bs_{'div' if only_div else 'all'}_{'pre' if from_prev else '1'}"
    return inner

r31_bs_all_1 = r31_bs(False, False)
r31_bs_div_1 = r31_bs(True, False)
r31_bs_all_pre = r31_bs(False, True)
r31_bs_div_pre = r31_bs(True, True)

solvers = [
    r30_dict,
    r30_acc_bs,
    r30_two_pointer,
    r31_bs_all_1,
    r31_bs_div_1,
    r31_bs_all_pre,
    r31_bs_div_pre,
]
