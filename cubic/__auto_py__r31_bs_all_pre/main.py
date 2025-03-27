#! /usr/bin/env python
import sys
from pathlib import Path
sol = Path(__file__).resolve().parents[2] / "solution_py"
sys.path.append(str(sol))

import e3
import utility

if __name__ == "__main__":
    utility.answer_cubic_with(e3.r31_bs_all_pre)
