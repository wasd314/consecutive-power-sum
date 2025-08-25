primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,]

def enumerate_smooth(ip: int, l: int, r: int):
    """l 以上 r 以下の primes[ip]-smooth な正整数を列挙する"""

    # ans = []
    ans = 0

    def dfs(i: int, n: int):
        if i == -1:
            nonlocal ans
            if l <= n <= r:
                # ans.append(n)
                ans += 1
            return
        p = primes[i]
        while n <= r:
            dfs(i - 1, n)
            n *= p
    dfs(ip, 1)
    # ans.sort()
    return ans

