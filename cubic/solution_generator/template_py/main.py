#! /usr/bin/env processor
import sys
from pathlib import Path
sol = Path(__file__).resolve().parents[2] / "solution_py"
sys.path.append(str(sol))

import ee
import utility

if __name__ == "__main__":
    utility.answer_cubic_with(ee.dummy)
