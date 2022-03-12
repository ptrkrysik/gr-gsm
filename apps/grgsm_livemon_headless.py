#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Gr-gsm Livemon
# Author: Piotr Krysik
# Description: Interactive monitor of a single C0 channel with analysis performed by Wireshark (command to run wireshark: sudo wireshark -k -f udp -Y gsmtap -i lo)
# GNU Radio version: v3.11.0.0git-46-g614681ba

from gnuradio import blocks
from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import network
from gsm import arfcn
from math import pi
import gsm
import pmt
import osmosdr
import time




class grgsm_livemon_headless(gr.top_block):

    def __init__(self, args="", collector="localhost", collectorport='4729', fc=925.8e6, gain=30, osr=4, ppm=0, rec_len=1000000, samp_rate=2000000.052982, serverport='4729', shiftoff=400e3):
        gr.top_block.__init__(self, "Gr-gsm Livemon", catch_exceptions=True)

        ##################################################
        # Parameters
        ##################################################
        self.args = args
        self.collector = collector
        self.collectorport = collectorport
        self.fc = fc
        self.gain = gain
        self.osr = osr
        self.ppm = ppm
        self.rec_len = rec_len
        self.samp_rate = samp_rate
        self.serverport = serverport
        self.shiftoff = shiftoff

        ##################################################
        # Blocks
        ##################################################
        self.rtlsdr_source_0 = osmosdr.source(
            args="numchan=" + str(1) + " " + 'str(gsm.device.get_default_args(args))'
        )
        self.rtlsdr_source_0.set_sample_rate(samp_rate)
        self.rtlsdr_source_0.set_center_freq(fc-shiftoff, 0)
        self.rtlsdr_source_0.set_freq_corr(ppm, 0)
        self.rtlsdr_source_0.set_dc_offset_mode(2, 0)
        self.rtlsdr_source_0.set_iq_balance_mode(2, 0)
        self.rtlsdr_source_0.set_gain_mode(False, 0)
        self.rtlsdr_source_0.set_gain(gain, 0)
        self.rtlsdr_source_0.set_if_gain(20, 0)
        self.rtlsdr_source_0.set_bb_gain(20, 0)
        self.rtlsdr_source_0.set_antenna('', 0)
        self.rtlsdr_source_0.set_bandwidth(250e3+abs(shiftoff), 0)
        self.network_socket_pdu_1 = network.socket_pdu('UDP_SERVER', '127.0.0.1', serverport, 1500, False)
        self.network_socket_pdu_0 = network.socket_pdu('UDP_CLIENT', collector, collectorport, 1500, False)
        self.gsm_sdcch8_demapper_0 = gsm.gsm_sdcch8_demapper(
            timeslot_nr=1,
        )
        self.gsm_receiver_0 = gsm.receiver(osr, [arfcn.downlink2arfcn(fc)], [], False)
        self.gsm_message_printer_1 = gsm.message_printer(pmt.intern(""), False,
            False, False)
        self.gsm_input_0 = gsm.gsm_input(
            ppm=ppm-int(ppm),
            osr=osr,
            fc=fc,
            samp_rate_in=samp_rate,
        )
        self.gsm_decryption_0 = gsm.decryption([], 1)
        self.gsm_control_channels_decoder_0_0 = gsm.control_channels_decoder()
        self.gsm_control_channels_decoder_0 = gsm.control_channels_decoder()
        self.gsm_clock_offset_control_0 = gsm.clock_offset_control(fc-shiftoff, samp_rate, osr)
        self.gsm_bcch_ccch_sdcch4_demapper_0 = gsm.gsm_bcch_ccch_sdcch4_demapper(
            timeslot_nr=0,
        )
        self.blocks_rotator_cc_0 = blocks.rotator_cc(-2*pi*shiftoff/samp_rate, False)
        self.blocks_head_0 = blocks.head(gr.sizeof_gr_complex*1, int(rec_len*samp_rate))


        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.gsm_bcch_ccch_sdcch4_demapper_0, 'bursts'), (self.gsm_control_channels_decoder_0, 'bursts'))
        self.msg_connect((self.gsm_clock_offset_control_0, 'ctrl'), (self.gsm_input_0, 'ctrl_in'))
        self.msg_connect((self.gsm_control_channels_decoder_0, 'msgs'), (self.network_socket_pdu_0, 'pdus'))
        self.msg_connect((self.gsm_control_channels_decoder_0_0, 'msgs'), (self.network_socket_pdu_0, 'pdus'))
        self.msg_connect((self.gsm_decryption_0, 'bursts'), (self.gsm_control_channels_decoder_0_0, 'bursts'))
        self.msg_connect((self.gsm_receiver_0, 'C0'), (self.gsm_bcch_ccch_sdcch4_demapper_0, 'bursts'))
        self.msg_connect((self.gsm_receiver_0, 'measurements'), (self.gsm_clock_offset_control_0, 'measurements'))
        self.msg_connect((self.gsm_receiver_0, 'C0'), (self.gsm_sdcch8_demapper_0, 'bursts'))
        self.msg_connect((self.gsm_sdcch8_demapper_0, 'bursts'), (self.gsm_decryption_0, 'bursts'))
        self.msg_connect((self.network_socket_pdu_1, 'pdus'), (self.gsm_message_printer_1, 'msgs'))
        self.connect((self.blocks_head_0, 0), (self.blocks_rotator_cc_0, 0))
        self.connect((self.blocks_rotator_cc_0, 0), (self.gsm_input_0, 0))
        self.connect((self.gsm_input_0, 0), (self.gsm_receiver_0, 0))
        self.connect((self.rtlsdr_source_0, 0), (self.blocks_head_0, 0))


    def get_args(self):
        return self.args

    def set_args(self, args):
        self.args = args

    def get_collector(self):
        return self.collector

    def set_collector(self, collector):
        self.collector = collector

    def get_collectorport(self):
        return self.collectorport

    def set_collectorport(self, collectorport):
        self.collectorport = collectorport

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.gsm_clock_offset_control_0.set_fc(self.fc-self.shiftoff)
        self.gsm_input_0.set_fc(self.fc)
        self.rtlsdr_source_0.set_center_freq(self.fc-self.shiftoff, 0)

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain
        self.rtlsdr_source_0.set_gain(self.gain, 0)

    def get_osr(self):
        return self.osr

    def set_osr(self, osr):
        self.osr = osr
        self.gsm_input_0.set_osr(self.osr)

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.gsm_input_0.set_ppm(self.ppm-int(self.ppm))
        self.rtlsdr_source_0.set_freq_corr(self.ppm, 0)

    def get_rec_len(self):
        return self.rec_len

    def set_rec_len(self, rec_len):
        self.rec_len = rec_len
        self.blocks_head_0.set_length(int(self.rec_len*self.samp_rate))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_head_0.set_length(int(self.rec_len*self.samp_rate))
        self.blocks_rotator_cc_0.set_phase_inc(-2*pi*self.shiftoff/self.samp_rate)
        self.gsm_input_0.set_samp_rate_in(self.samp_rate)
        self.rtlsdr_source_0.set_sample_rate(self.samp_rate)

    def get_serverport(self):
        return self.serverport

    def set_serverport(self, serverport):
        self.serverport = serverport

    def get_shiftoff(self):
        return self.shiftoff

    def set_shiftoff(self, shiftoff):
        self.shiftoff = shiftoff
        self.blocks_rotator_cc_0.set_phase_inc(-2*pi*self.shiftoff/self.samp_rate)
        self.gsm_clock_offset_control_0.set_fc(self.fc-self.shiftoff)
        self.rtlsdr_source_0.set_center_freq(self.fc-self.shiftoff, 0)
        self.rtlsdr_source_0.set_bandwidth(250e3+abs(self.shiftoff), 0)



def argument_parser():
    description = 'Interactive monitor of a single C0 channel with analysis performed by Wireshark (command to run wireshark: sudo wireshark -k -f udp -Y gsmtap -i lo)'
    parser = ArgumentParser(description=description)
    parser.add_argument(
        "--args", dest="args", type=str, default="",
        help="Set Device Arguments [default=%(default)r]")
    parser.add_argument(
        "--collector", dest="collector", type=str, default="localhost",
        help="Set IP or DNS name of collector point [default=%(default)r]")
    parser.add_argument(
        "--collectorport", dest="collectorport", type=str, default='4729',
        help="Set UDP port number of collector [default=%(default)r]")
    parser.add_argument(
        "-f", "--fc", dest="fc", type=eng_float, default=eng_notation.num_to_str(float(925.8e6)),
        help="Set GSM channel's central frequency [default=%(default)r]")
    parser.add_argument(
        "-g", "--gain", dest="gain", type=eng_float, default=eng_notation.num_to_str(float(30)),
        help="Set RF Gain [default=%(default)r]")
    parser.add_argument(
        "--osr", dest="osr", type=intx, default=4,
        help="Set OverSampling Ratio [default=%(default)r]")
    parser.add_argument(
        "-p", "--ppm", dest="ppm", type=eng_float, default=eng_notation.num_to_str(float(0)),
        help="Set Clock frequency offset in ppms (1/1e6 parts) [default=%(default)r]")
    parser.add_argument(
        "-T", "--rec-len", dest="rec_len", type=eng_float, default=eng_notation.num_to_str(float(1000000)),
        help="Set Recording length in seconds [default=%(default)r]")
    parser.add_argument(
        "-s", "--samp-rate", dest="samp_rate", type=eng_float, default=eng_notation.num_to_str(float(2000000.052982)),
        help="Set samp_rate [default=%(default)r]")
    parser.add_argument(
        "--serverport", dest="serverport", type=str, default='4729',
        help="Set UDP server listening port [default=%(default)r]")
    parser.add_argument(
        "-o", "--shiftoff", dest="shiftoff", type=eng_float, default=eng_notation.num_to_str(float(400e3)),
        help="Set Frequency Shiftoff [default=%(default)r]")
    return parser


def main(top_block_cls=grgsm_livemon_headless, options=None):
    if options is None:
        options = argument_parser().parse_args()
    tb = top_block_cls(args=options.args, collector=options.collector, collectorport=options.collectorport, fc=options.fc, gain=options.gain, osr=options.osr, ppm=options.ppm, rec_len=options.rec_len, samp_rate=options.samp_rate, serverport=options.serverport, shiftoff=options.shiftoff)

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
