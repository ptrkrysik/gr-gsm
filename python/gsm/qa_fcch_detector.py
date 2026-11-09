#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2014 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import unittest

import numpy as np
import pmt
from gnuradio import gr, gr_unittest
from gnuradio import blocks
from gnuradio import gsm

# fcch_detector is an experimental component and needs scipy
HAVE_FCCH_DETECTOR = hasattr(gsm, "fcch_detector")


@unittest.skipUnless(HAVE_FCCH_DETECTOR, "fcch_detector needs scipy")
class qa_fcch_detector (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_fcch_tag_and_frequency_offset (self):
        osr = 4
        symb_rate = 1625000.0 / 6.0
        samp_rate = symb_rate * osr
        freq_offset = 500.0

        rng = np.random.default_rng(1)

        def noise(n):
            return ((rng.normal(size=n) + 1j * rng.normal(size=n)) * 0.05).astype(np.complex64)

        # FCCH burst is a pure tone at a quarter of the symbol rate
        n = np.arange(int(148 * osr))
        fcch = np.exp(2j * np.pi * (symb_rate / 4 + freq_offset) * n / samp_rate).astype(np.complex64)
        data = np.concatenate([noise(3000), fcch, noise(3000), fcch, noise(3000), fcch, noise(3000)])

        src = blocks.vector_source_c(data.tolist(), False)
        dut = gsm.fcch_detector(osr)
        snk = blocks.vector_sink_c()
        self.tb.connect(src, dut, snk)
        self.tb.run()

        tags = [t for t in snk.tags() if pmt.symbol_to_string(t.key) == "fcch"]
        self.assertGreaterEqual(len(tags), 1)
        for t in tags:
            self.assertAlmostEqual(pmt.to_double(t.value), freq_offset, delta=5.0)
            # the tag is put at the beginning of a burst (within a burst length of its end)
            burst_ends = [3000 + 148 * osr + k * (3000 + 148 * osr) for k in range(3)]
            self.assertTrue(any(abs(t.offset - e) < 40 for e in burst_ends), t.offset)


if __name__ == '__main__':
    gr_unittest.run(qa_fcch_detector)
