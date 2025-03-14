#! /usr/local/bin/python3

def solve(n: int):
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
            ans.append((l, r))
    ans.sort()
    return ans

if __name__ == "__main__":
    n = int(input())
    ans = solve(n)
    print(len(ans))
    for t in ans:
        print(*t)

