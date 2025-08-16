#! /usr/bin/env python
import sys
import consts

content = sys.stdin.read()
n = int(content.rstrip())
assert consts.MIN_N <= n <= consts.MAX_N
assert content == f"{n}\n"
