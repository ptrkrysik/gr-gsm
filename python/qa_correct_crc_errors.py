#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @file
# @author Vasil Velichkov <vvvelichkov@gmail.com>
# @section LICENSE
#
# Gr-gsm is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# Gr-gsm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gr-gsm; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#
#

from gnuradio import gr, gr_unittest, blocks
import grgsm_swig as grgsm
import pmt

class qa_correct_crc_errors (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_correct_crc_error (self):
        """
                SMS bursts from uplink_decoding_multi_rtl.py
        """

        framenumbers_input = [
                2543542, 2543543, 2543544, 2543545]
        timeslots_input = [
                1, 1, 1, 1]
        bursts_input = [
                "1001010010101010110000101000110011110010001011011111101011101010000111011101001000011101110011010001011011001001100010011011000010010110100010001000",
                "0001010010111010110111000010111110010111010011110110010001101010000111011101001000011101101101111110001100001100110000000011110001001010101100100000",
                "0001001111101001011111010001101100011110100111110001001110101010000111011101001000011101011111001100011011001001001001001100100000111000011000110000",
                "0001111010111101011010011011110101110001000001001011110101011010000111011101001000011101100000111100100010001100110110011010001001011110011001001000"
                #2543542 3928686:  0d 04 45 d9 6e b8 5c 0c 16 8b c5 64 b1 59 8c 16 ab c5 6c 6b 2b 2b 2b
        ]

        src = grgsm.burst_source(framenumbers_input, timeslots_input, bursts_input)
        decoder = grgsm.control_channels_decoder()
        dst = grgsm.message_sink()

        self.tb.msg_connect(src, "out", decoder, "bursts")
        self.tb.msg_connect(decoder, "msgs", dst, "in")

        self.tb.run ()

        # have to convert tuple to list
        messages = list(dst.get_messages())

        expected = [
                "02 04 01 01 00 00 00 00 00 26 cf b6 06 00 00 00 0d 04 45 d9 6e b8 5c 0c 16 8b c5 64 b1 59 8c 16 ab c5 6c 6b 2b 2b 2b"
        ]
        self.assertEqual(expected, messages)


if __name__ == '__main__':
    gr_unittest.run(qa_correct_crc_errors, "qa_correct_crc_errors.xml")
