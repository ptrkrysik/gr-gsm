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

class qa_control_channels_decoder (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001 (self):
        """
                SMS bursts from uplink_decoding_multi_rtl.py
        """

        framenumbers_input = [
                2543440, 2543441, 2543442, 2543443,
                2543491, 2543492, 2543493, 2543494]
        timeslots_input = [
                1, 1, 1, 1,
                1, 1, 1, 1]
        bursts_input = [
                "0001100001110111110010000000101110000110000001111010011000001010000111011101001000011101011011111111101010000101111100101101101100011100100001100000",
                "1000000000000000101000110100011100000010000011100011000010101010000111011101001000010010100001101110000011001111010011100101100101110101000100110000",
                "0000100110010001001000011000011100100110000011001010000110111010000111011101001000011101010101100001001010011000101110010100110101111111010000100000",
                "0001011011001001010100111010010011100010001100001101000100101010000111011101001000011101110010001000001001011001011111100110101000100110010000101000",
                #2543440 3928688:  0d 00 53 29 01 36 00 04 00 07 91 84 97 90 89 52 f0 2a 31 02 09 81 35

                "0001010011100101100100011001111011000110011000111111011000011010000111011101001000011101010000111101001000100100110010011110111101010100101100000000",
                "0001100101101000111111100001010100010010010011100100101101011010000111011101001000011101101010101100010111111011101001100011110011110000010001100000",
                "0000011111010101011100011000111001010010010110111111001101101010000111011101001000011101010110111001111000111011001110001100011011001111000100010000",
                "0001001110011010100100110010101110000011111000101101000111101010000111011101001000011101010010100110100000000110100010000001011110111111100010000000",
                #2543491 3928687:  0d 02 53 30 61 00 f8 00 00 a7 21 d4 f2 9c 0e 9a b7 e7 a0 98 6c 46 ab
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
                "02 04 01 01 00 00 00 00 00 26 cf 50 06 00 00 00 0d 00 53 29 01 36 00 04 00 07 91 84 97 90 89 52 f0 2a 31 02 09 81 35",
                "02 04 01 01 00 00 00 00 00 26 cf 83 06 00 00 00 0d 02 53 30 61 00 f8 00 00 a7 21 d4 f2 9c 0e 9a b7 e7 a0 98 6c 46 ab",
        ]
        self.assertEqual(expected, messages)


if __name__ == '__main__':
    gr_unittest.run(qa_control_channels_decoder, "qa_control_channels_decoder.xml")
