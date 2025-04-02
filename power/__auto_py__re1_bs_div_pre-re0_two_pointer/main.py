#! /usr/bin/env pypy
import sys
from pathlib import Path
sol = Path(__file__).resolve().parents[2] / "solution_py"
sys.path.append(str(sol))

import e2
import utility

if __name__ == "__main__":
    utility.answer_power_with(e2.solvers_ac[3])
