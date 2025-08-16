import ee

def dummy(*args):
    pass

solvers = [
    ee.re0_dict,
    ee.re0_acc_bs,
    ee.re0_two_pointer,
    ee.re1_bs_all_1,
    ee.re1_bs_div_1,
    ee.re1_bs_all_pre,
    ee.re1_bs_div_pre,
]
