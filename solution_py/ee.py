from utility import power_sum, min_true
import bisect as bs

def dummy(*args):
    pass

def re0_dict(n: int, e: int):
    pows = []
    for i in range(n + 1):
        if i**e <= n:
            pows.append(i**e)
        else:
            break
    # pows[i] = i**e

    c = len(pows)
    acc = [0] * (c + 1)
    for i in range(c):
        acc[i + 1] = acc[i] + pows[i]
    # acc[i] = sum(j**e for j < i)
    # acc[i + 1] = sum(j**e for j <= i)

    right = {acc[i + 1]: i for i in range(1, c)}
    ans = []
    for l in range(1, c + 1):
        r = right.get(acc[l] + n, -1)
        if r != -1:
            ans.append((e, l, r))
    ans.sort()
    return ans

def re0_acc_bs(n: int, e: int):
    pows = []
    for i in range(n + 1):
        if i**e <= n:
            pows.append(i**e)
        else:
            break
    # pows[i] = i**e

    c = len(pows)
    acc = [0] * (c + 1)
    for i in range(c):
        acc[i + 1] = acc[i] + pows[i]
    # acc[i] = sum(j**e for j < i)
    # acc[i + 1] = sum(j**e for j <= i)

    ans = []
    for l in range(1, c + 1):
        r = bs.bisect_left(acc, acc[l] + n)
        if r == c + 1:
            break
        if acc[r] - acc[l] == n:
            ans.append((e, l, r - 1))
    ans.sort()
    return ans

def re0_two_pointer(n: int, e: int):
    pows = []
    for i in range(n + 1):
        if i**e <= n:
            pows.append(i**e)
        else:
            break
    # pows[i] = i**e

    c = len(pows)
    ans = []
    r = 1
    current_sum = 0
    for l in range(1, c):
        while r < c and current_sum + pows[r] <= n:
            current_sum += pows[r]
            r += 1
        # (r >= c or) current_sum <= n < current_sum + pows[r]
        if current_sum == n:
            ans.append((e, l, r - 1))
        if l < r:
            current_sum -= pows[l]
    ans.sort()
    return ans

enough_denom = [1, 2, 6, 2, 30, 2, 42, 2, 30, 2, 66, 2, 2730, 2, 6, 2, 510, 2, 798, 2, 330, 2, 138, 2, 2730, 2, 6, 2, 870, 2, 14322, 2, 510, 2, 6, 2, 1919190, 2, 6, 2, 13530, 2, 1806, 2, 690, 2, 282, 2, 46410, 2, 66, 2, 1590, 2, 798, 2, 870, 2, 354, 2, 56786730, 2, 6, 2, 510, 2, 64722, 2, 30, 2, 4686, 2, 140100870, 2, 6, 2, 30, 2, 3318, 2, 230010, 2, 498, 2, 3404310, 2, 6, 2, 61410, 2, 272118, 2, 1410, 2, 6, 2, 4501770, 2, 6, 2]

def re1_bs(only_div: bool, from_prev: bool):
    def inner(n: int, e: int):
        ans = []
        prev_l = n
        for w in range(1, n + 1):
            if power_sum(e, 1, w + 1) > n:
                break
            if only_div and enough_denom[e] * n % w:
                continue

            def pred(l):
                return power_sum(e, l, l + w) >= n
            if from_prev:
                dl = 1
                while dl < prev_l and pred(prev_l - dl):
                    dl <<= 1
                l = min_true(max(0, prev_l - dl), prev_l + 1, pred)
                prev_l = l
            else:
                r = 1
                while not pred(r):
                    r <<= 1
                l = min_true(1, r + 1, pred)
            if power_sum(e, l, l + w) == n:
                ans.append((e, l, l + w - 1))
        ans.reverse()
        ans.sort()
        return ans
    inner.__name__ = f"re1_bs_{'div' if only_div else 'all'}_{'pre' if from_prev else '1'}"
    return inner

re1_bs_all_1 = re1_bs(False, False)
re1_bs_div_1 = re1_bs(True, False)
re1_bs_all_pre = re1_bs(False, True)
re1_bs_div_pre = re1_bs(True, True)

re0 = [
    re0_two_pointer,
    re0_acc_bs,
    re0_dict,
]
re1 = [
    re1_bs_all_1,
    re1_bs_div_1,
    re1_bs_all_pre,
    re1_bs_div_pre,
]

solvers = re0 + re1
