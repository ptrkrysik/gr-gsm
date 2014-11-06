#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: GSM input adaptor
# Author: Piotr Krysik
# Description: Adaptor of input stream for the GSM receiver. Contains frequency ofset corrector doing also resampling to integer multiplies of GSM sample rate and LP filter filtering GSM channel.
# Generated: Thu Nov  6 14:41:06 2014
##################################################

from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes
import gsm

class gsm_input(gr.hier_block2):

    def __init__(self, ppm=0, osr=4, fc=940e6, samp_rate_in=1e6):
        gr.hier_block2.__init__(
            self, "GSM input adaptor",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.ppm = ppm
        self.osr = osr
        self.fc = fc
        self.samp_rate_in = samp_rate_in

        ##################################################
        # Blocks
        ##################################################
        self.ppm_in = None;self.message_port_register_hier_out("ppm_in")
        self.low_pass_filter_0_0 = filter.fir_filter_ccf(1, firdes.low_pass(
        	1, 1625000.0/6.0*osr, 125e3, 5e3, firdes.WIN_HAMMING, 6.76))
        self.gsm_clock_offset_corrector_0 = gsm.clock_offset_corrector(
            fc=fc,
            ppm=0,
            samp_rate_in=samp_rate_in,
            samp_rate_out=1625000.0/6.0*4.0,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.low_pass_filter_0_0, 0), (self, 0))
        self.connect((self.gsm_clock_offset_corrector_0, 0), (self.low_pass_filter_0_0, 0))
        self.connect((self, 0), (self.gsm_clock_offset_corrector_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self, "ppm_in", self.gsm_clock_offset_corrector_0, "ppm_in")


    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm

    def get_osr(self):
        return self.osr

    def set_osr(self, osr):
        self.osr = osr
        self.low_pass_filter_0_0.set_taps(firdes.low_pass(1, 1625000.0/6.0*self.osr, 125e3, 5e3, firdes.WIN_HAMMING, 6.76))

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.gsm_clock_offset_corrector_0.set_fc(self.fc)

    def get_samp_rate_in(self):
        return self.samp_rate_in

    def set_samp_rate_in(self, samp_rate_in):
        self.samp_rate_in = samp_rate_in
        self.gsm_clock_offset_corrector_0.set_samp_rate_in(self.samp_rate_in)

