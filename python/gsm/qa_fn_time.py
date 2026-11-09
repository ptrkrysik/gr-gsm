#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2017 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from random import Random

from gnuradio import gr_unittest
from gnuradio import gsm

HYPER_FRAME = 26 * 51 * 2048
SYMB_RATE = 13.0e6 / 48.0
TS_PERIOD = 156.25 / SYMB_RATE
FRAME_PERIOD = 8 * TS_PERIOD


class qa_fn_time (gr_unittest.TestCase):
    """
    Checks that the C++ and the Python implementation of fn_time_delta
    agree, and that they recover the frame number difference from a coarse
    time hint.
    """

    def test_001_python_vs_cpp (self):
        rnd = Random(1)
        fn1 = 10000
        ts_ref = 4
        time1 = 10.5
        for fn2 in range(HYPER_FRAME // 2 + fn1 - 10, HYPER_FRAME // 2 * 10 + fn1 + 100, 10):
            ts_x = int(rnd.uniform(0, 8))
            time2 = time1 + (fn2 - fn1) * FRAME_PERIOD + (ts_x - ts_ref) * TS_PERIOD
            error = rnd.uniform(-6200, 6200)
            time2_err = time2 + error

            fn_delta, time2_precise = gsm.fn_time_delta(fn1, time1, fn2, time2_err, ts_x, ts_ref)
            self.assertEqual(fn_delta, fn2 - fn1)

            time2_precise_cpp = gsm.fn_time_delta_cpp(
                fn1, (int(time1), time1 - int(time1)),
                fn2, (int(time2_err), time2_err - int(time2_err)),
                ts_x, ts_ref)
            self.assertAlmostEqual(time2_precise_cpp[0] + time2_precise_cpp[1], time2_precise, delta=1e-4)


if __name__ == '__main__':
    gr_unittest.run(qa_fn_time)
