#! /usr/bin/env python
import sys
import os
from pathlib import Path
import shutil

here = Path(__file__).resolve().parent
problem_root = here.parent
sol = problem_root.parent / "solution_py"
template_root = here / "template_py"

sys.path.append(str(sol))

import e3
auto_prefix = "__auto_py__"

def remove_prev():
    dirs = [d for d in os.listdir(problem_root) if os.path.isdir(problem_root / d) and d.startswith(auto_prefix)]
    for d in dirs:
        shutil.rmtree(str(problem_root / d))
        print("removed:", d)

def gen(solver, processor):
    name = str(solver.__name__)
    target_root = problem_root / f"{auto_prefix}{name}"
    os.makedirs(str(target_root), exist_ok=True)
    with open(str(template_root / "SOLUTION")) as src:
        with open(str(target_root / "SOLUTION"), "w") as dst:
            dst.write(src.read())
    with open(str(template_root / "main.py")) as src:
        with open(str(target_root / "main.py"), "w") as dst:
            dst.write(src.read().replace("dummy", name).replace("processor", processor))
    print("generated:", processor, name)


def main():
    if len(sys.argv) < 2 or sys.argv[1] not in ["python", "pypy"]:
        print("Specify processor (python or pypy) like:  $ python gen_py.py python")
        exit(1)
    remove_prev()
    for solver in e3.solvers:
        gen(solver, sys.argv[1])

if __name__ == "__main__":
    main()

