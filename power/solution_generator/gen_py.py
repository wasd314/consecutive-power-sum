#! /usr/bin/env python
import sys
import os
from pathlib import Path
import shutil

here = Path(__file__).resolve().parent
problem_root = here.parent
sol = problem_root.parent / "solution_py"
template_root = dict(ac=here / "template_py_ac", tle=here / "template_py_tle")

sys.path.append(str(sol))

import utility
import e2
auto_prefix = "__auto_py__"
solvers = dict(ac=e2.solvers_ac, tle=e2.solvers_tle)

def remove_prev():
    dirs = [d for d in os.listdir(problem_root) if os.path.isdir(problem_root / d) and d.startswith(auto_prefix)]
    for d in dirs:
        shutil.rmtree(str(problem_root / d))
        print("removed:", d)

def gen(ty: str, i: int, processor: str):
    solver = solvers[ty][i]
    name = solver.name
    target_root = problem_root / f"{auto_prefix}{name}"
    os.makedirs(str(target_root), exist_ok=True)
    with open(str(template_root[ty] / "SOLUTION")) as src:
        with open(str(target_root / "SOLUTION"), "w") as dst:
            dst.write(src.read())
    with open(str(template_root[ty] / "main.py")) as src:
        with open(str(target_root / "main.py"), "w") as dst:
            dst.write(src.read().replace("e2.solvers_ac[0]", f"e2.solvers_{ty}[{i}]").replace("processor", processor))
    print("generated:", processor, name)


def main():
    if len(sys.argv) < 2 or sys.argv[1] not in ["python", "pypy"]:
        print("Specify processor (python or pypy) like:  $ python gen_py.py python")
        exit(1)
    remove_prev()
    proc = sys.argv[1]

    # ac
    line = input(f"ac[{len(solvers['ac'])}]: ")
    todo = utility.parse_range(len(solvers["ac"]), line)
    for i in todo:
        gen("ac", i, proc)

    # tle
    line = input(f"tle[{len(solvers['tle'])}]: ")
    todo = utility.parse_range(len(solvers["tle"]), line)
    for i in todo:
        gen("tle", i, proc)

if __name__ == "__main__":
    main()

