#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import os
import re

from gnuradio import gr_unittest

# 3GPP TS 45.002, table 5.2.3a (TSC Set 1, GMSK), bits BN61..BN86
TSC_SET_1 = [
    "00100101110000100010010111",
    "00101101110111100010110111",
    "01000011101110100100001110",
    "01000111101101000100011110",
    "00011010111001000001101011",
    "01001110101100000100111010",
    "10100111110110001010011111",
    "11101111000100101110111100",
]

# 3GPP TS 45.002, table 5.2.5-3, bits BN42..BN105
SYNC_BITS = "1011100101100010000001000000111100101101010001010111011000011011"

# 3GPP TS 45.002, 5.2.6, mixed bits of the dummy burst, BN61..BN86 (the part
# that overlaps the training sequence position of a normal burst)
DUMMY_TS = "01110001011100010111000101"


def read_constants():
    here = os.path.dirname(os.path.abspath(__file__))
    path = os.path.join(here, "..", "..", "include", "gnuradio", "gsm", "gsm_constants.h")
    with open(path) as f:
        return f.read()


def bits(text):
    return "".join(re.findall("[01]", text))


class qa_training_sequences(gr_unittest.TestCase):

    def test_train_seq(self):
        text = read_constants()
        body = re.search(r"train_seq\[TRAIN_SEQ_NUM\]\[N_TRAIN_BITS\]\s*=\s*\{(.*?)\n\};",
                         text, re.S).group(1)
        rows = [bits(r) for r in re.findall(r"\{([^}]*)\}", body)]
        self.assertEqual(9, len(rows))
        for tsc, expected in enumerate(TSC_SET_1):
            self.assertEqual(expected, rows[tsc], "TSC%d" % tsc)
        self.assertEqual(DUMMY_TS, rows[8], "dummy burst")

    def test_sync_bits(self):
        text = read_constants()
        body = re.search(r"SYNC_BITS\[\]\s*=\s*\{(.*?)\};", text, re.S).group(1)
        self.assertEqual(SYNC_BITS, bits(body))


if __name__ == '__main__':
    gr_unittest.run(qa_training_sequences)
