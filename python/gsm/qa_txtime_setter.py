#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2017 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import struct
import time

import pmt
from gnuradio import gr, gr_unittest
from gnuradio import blocks
from gnuradio import gsm

UNKNOWN_FN = 0xffffffff
SYMB_RATE = 13.0e6 / 48.0
FRAME_PERIOD = 8 * 156.25 / SYMB_RATE


def make_burst_msg(fn, ts):
    # gsmtap header (16 bytes) followed by a burst (all zeros)
    header = struct.pack(">BBBBHbbIBBBB", 2, 4, 1, ts, 0, 0, 0, fn, 0, 0, 0, 0)
    blob = pmt.init_u8vector(len(header) + 148, list(header) + [0] * 148)
    return pmt.cons(pmt.make_dict(), blob)


def make_fn_time_msg(fn, ts, secs, fracs):
    fn_time = pmt.cons(pmt.cons(pmt.from_uint64(fn), pmt.from_uint64(ts)),
                       pmt.cons(pmt.from_uint64(secs), pmt.from_double(fracs)))
    return pmt.dict_add(pmt.make_dict(), pmt.intern("fn_time"), fn_time)


def make_time_hint_msg(secs, fracs):
    time_hint = pmt.cons(pmt.from_uint64(secs), pmt.from_double(fracs))
    return pmt.dict_add(pmt.make_dict(), pmt.intern("time_hint"), time_hint)


class qa_txtime_setter (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()
        self.dbg = blocks.message_debug()

    def tearDown (self):
        self.tb = None

    def make_dut(self, init_fn, secs=100, fracs=0.0, timing_advance=0.0, delay_correction=0.0):
        dut = gsm.txtime_setter(init_fn, secs, fracs, secs, fracs, timing_advance, delay_correction)
        self.tb.msg_connect(dut, "bursts_out", self.dbg, "store")
        self.tb.start()
        return dut

    def post(self, dut, port, msg):
        dut.to_basic_block()._post(pmt.intern(port), msg)
        # GNU Radio keeps a queue per message port and does not order the messages of
        # different ports, so wait until the block has handled this one before the next
        # is posted, otherwise the tests that mix the ports depend on the scheduling
        time.sleep(0.3)

    def finish(self, expected_msgs):
        # bursts that are dropped never show up, so give the block some time
        # to process everything that has been posted
        for _ in range(100):
            if self.dbg.num_messages() >= expected_msgs:
                break
            time.sleep(0.02)
        time.sleep(0.2)
        self.tb.stop()
        self.tb.wait()
        self.assertEqual(self.dbg.num_messages(), expected_msgs)

    def check_tags(self, msg, fn, ts, txtime):
        meta = pmt.car(msg)
        self.assertEqual(pmt.to_uint64(pmt.dict_ref(meta, pmt.intern("fn"), pmt.PMT_NIL)), fn)
        self.assertEqual(pmt.to_uint64(pmt.dict_ref(meta, pmt.intern("ts"), pmt.PMT_NIL)), ts)
        tx_time = pmt.dict_ref(meta, pmt.intern("tx_time"), pmt.PMT_NIL)
        txtime_got = pmt.to_uint64(pmt.tuple_ref(tx_time, 0)) + pmt.to_double(pmt.tuple_ref(tx_time, 1))
        self.assertAlmostEqual(txtime_got, txtime, places=6)

    def test_001_timing_of_bursts (self):
        dut = self.make_dut(1000, timing_advance=0.002, delay_correction=0.001)
        self.post(dut, "bursts_in", make_burst_msg(1010, 3))
        self.finish(1)
        expected = 100.0 + 10 * FRAME_PERIOD + 3 * FRAME_PERIOD / 8 - 0.003
        self.check_tags(self.dbg.get_message(0), 1010, 3, expected)

    def test_002_late_and_early_bursts_are_dropped (self):
        dut = self.make_dut(1000)
        self.post(dut, "bursts_in", make_burst_msg(999, 0))       # before the time hint
        self.post(dut, "bursts_in", make_burst_msg(1010, 0))      # ok
        self.post(dut, "bursts_in", make_burst_msg(1000 + 3000, 0))  # more than 10 s ahead
        self.finish(1)
        self.check_tags(self.dbg.get_message(0), 1010, 0, 100.0 + 10 * FRAME_PERIOD)

    def test_003_unknown_reference_then_reference_set (self):
        dut = self.make_dut(UNKNOWN_FN)
        self.post(dut, "bursts_in", make_burst_msg(1010, 0))      # dropped, no reference
        self.post(dut, "fn_time", make_fn_time_msg(2000, 0, 200, 0.5))
        self.post(dut, "bursts_in", make_burst_msg(2010, 0))
        self.finish(1)
        self.check_tags(self.dbg.get_message(0), 2010, 0, 200.5 + 10 * FRAME_PERIOD)

    def test_004_time_hint_message (self):
        dut = self.make_dut(1000)
        self.post(dut, "fn_time", make_time_hint_msg(100, 0.5))
        self.post(dut, "bursts_in", make_burst_msg(1005, 0))      # before the new time hint, dropped
        self.post(dut, "bursts_in", make_burst_msg(1200, 0))
        self.finish(1)
        self.check_tags(self.dbg.get_message(0), 1200, 0, 100.0 + 200 * FRAME_PERIOD)

    def test_005_setters (self):
        dut = self.make_dut(1000)
        dut.set_delay_correction(0.01)
        dut.set_timing_advance(0.02)
        dut.set_fn_time_reference(5000, 0, 300, 0.0)
        dut.set_time_hint(300, 0.0)
        self.post(dut, "bursts_in", make_burst_msg(5010, 0))
        self.finish(1)
        self.check_tags(self.dbg.get_message(0), 5010, 0, 300.0 + 10 * FRAME_PERIOD - 0.03)


if __name__ == '__main__':
    gr_unittest.run(qa_txtime_setter)
