#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2017 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import time

import pmt
from gnuradio import gr, gr_unittest
from gnuradio import blocks
from gnuradio import gsm


class qa_txtime_bursts_tagger (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        tb  = self.tb
        time_ref = 0
        fn_ref = 0
        dut = gsm.txtime_bursts_tagger(fn_ref, time_ref)

        framenumbers_input = [1259192, 1076346, 1076242, 235879, 1259218]
        timeslots_input = [6, 3, 4, 3, 5]
        bursts_input = [
            "0001100001000111100111101111100101000100101011000010011110011101001111101100010100111111100000110100011111101011101100100111110011000100010001010000",
            "0001000101000000001001111110000110010110110111110111101000001101001111101100010100111111001110001001110101110001010001000111011010010001011011000000",
            "0001001101101101000111001000101011001101001110110001001100111101001111101100010100111111111001001010011010011111010010010101011001001011011100110000",
            "0000010010100000001001101010100001011100010001101100111111101101001111101100010100111111101101001110100010101110010110101111100010010000110010110000",
        ]
        n_bursts = len(bursts_input)

        src = gsm.burst_source(framenumbers_input, timeslots_input, bursts_input)
        dbg = blocks.message_debug()

        tb.msg_connect(src, "out", dut, "bursts")
        tb.msg_connect(dut, "bursts", dbg, "store")

        tb.start()
        for _ in range(100):
            if dbg.num_messages() >= n_bursts:
                break
            time.sleep(0.05)
        tb.stop()
        tb.wait()

        self.assertEqual(dbg.num_messages(), n_bursts)
        for i in range(n_bursts):
            meta = pmt.car(dbg.get_message(i))
            fn = pmt.to_uint64(pmt.dict_ref(meta, pmt.intern("fn"), pmt.PMT_NIL))
            self.assertEqual(fn, framenumbers_input[i])

            tx_time = pmt.dict_ref(meta, pmt.intern("tx_time"), pmt.PMT_NIL)
            txtime_secs = pmt.to_uint64(pmt.tuple_ref(tx_time, 0))
            txtime_fracs = pmt.to_double(pmt.tuple_ref(tx_time, 1))
            _, txtime_expected = gsm.fn_time_delta(fn_ref, time_ref, framenumbers_input[i],
                                                   time_ref, timeslots_input[i])
            self.assertAlmostEqual(txtime_secs + txtime_fracs, txtime_expected, places=6)


if __name__ == '__main__':
    gr_unittest.run(qa_txtime_bursts_tagger)
