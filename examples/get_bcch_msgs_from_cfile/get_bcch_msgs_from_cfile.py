#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Get BCCH messages from a cfile
# Author: Piotr Krysik
# GNU Radio version: 3.10.9.2

from gnuradio import blocks
import pmt
from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import gsm
from gnuradio import network




class get_bcch_msgs_from_cfile(gr.top_block):

    def __init__(self, fc=937e6, input_file_name="input.cfile", osr=4, samp_rate=(100.0e6/174.0)):
        gr.top_block.__init__(self, "Get BCCH messages from a cfile", catch_exceptions=True)

        ##################################################
        # Parameters
        ##################################################
        self.fc = fc
        self.input_file_name = input_file_name
        self.osr = osr
        self.samp_rate = samp_rate

        ##################################################
        # Blocks
        ##################################################

        self.network_socket_pdu_0 = network.socket_pdu('UDP_CLIENT', '127.0.0.1', '4729', 10000, False)
        self.gsm_receiver_0 = gsm.receiver(osr, [0], [], False)
        self.gsm_message_printer_0 = gsm.message_printer(pmt.intern(""), False,
            False, False)
        self.gsm_input_0 = gsm.gsm_input(fc=fc, osr=osr, ppm=0, samp_rate_in=samp_rate)
        self.gsm_control_channels_decoder_0 = gsm.control_channels_decoder()
        self.gsm_clock_offset_control_0 = gsm.clock_offset_control(fc, samp_rate, osr)
        self.gsm_bcch_ccch_demapper_0 = gsm.gsm_bcch_ccch_demapper(
            timeslot_nr=0,
        )
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_gr_complex*1, input_file_name, False, 0, 0)
        self.blocks_file_source_0.set_begin_tag(pmt.PMT_NIL)


        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.gsm_bcch_ccch_demapper_0, 'bursts'), (self.gsm_control_channels_decoder_0, 'bursts'))
        self.msg_connect((self.gsm_clock_offset_control_0, 'ctrl'), (self.gsm_input_0, 'ctrl_in'))
        self.msg_connect((self.gsm_control_channels_decoder_0, 'msgs'), (self.gsm_message_printer_0, 'msgs'))
        self.msg_connect((self.gsm_control_channels_decoder_0, 'msgs'), (self.network_socket_pdu_0, 'pdus'))
        self.msg_connect((self.gsm_receiver_0, 'C0'), (self.gsm_bcch_ccch_demapper_0, 'bursts'))
        self.msg_connect((self.gsm_receiver_0, 'measurements'), (self.gsm_clock_offset_control_0, 'measurements'))
        self.connect((self.blocks_file_source_0, 0), (self.gsm_input_0, 0))
        self.connect((self.gsm_input_0, 0), (self.gsm_receiver_0, 0))


    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.gsm_clock_offset_control_0.set_fc(self.fc)
        self.gsm_input_0.set_fc(self.fc)

    def get_input_file_name(self):
        return self.input_file_name

    def set_input_file_name(self, input_file_name):
        self.input_file_name = input_file_name
        self.blocks_file_source_0.open(self.input_file_name, False)

    def get_osr(self):
        return self.osr

    def set_osr(self, osr):
        self.osr = osr
        self.gsm_clock_offset_control_0.set_osr(self.osr)
        self.gsm_input_0.set_osr(self.osr)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.gsm_clock_offset_control_0.set_samp_rate(self.samp_rate)
        self.gsm_input_0.set_samp_rate_in(self.samp_rate)



def argument_parser():
    parser = ArgumentParser()
    parser.add_argument(
        "-f", "--fc", dest="fc", type=eng_float, default=eng_notation.num_to_str(float(937e6)),
        help="Set fc [default=%(default)r]")
    parser.add_argument(
        "-i", "--input-file-name", dest="input_file_name", type=str, default="input.cfile",
        help="Set input.cfile [default=%(default)r]")
    parser.add_argument(
        "--osr", dest="osr", type=intx, default=4,
        help="Set OSR [default=%(default)r]")
    parser.add_argument(
        "-s", "--samp-rate", dest="samp_rate", type=eng_float, default=eng_notation.num_to_str(float((100.0e6/174.0))),
        help="Set samp_rate [default=%(default)r]")
    return parser


def main(top_block_cls=get_bcch_msgs_from_cfile, options=None):
    if options is None:
        options = argument_parser().parse_args()
    tb = top_block_cls(fc=options.fc, input_file_name=options.input_file_name, osr=options.osr, samp_rate=options.samp_rate)

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        sys.exit(0)

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    tb.start()

    tb.wait()


if __name__ == '__main__':
    main()
