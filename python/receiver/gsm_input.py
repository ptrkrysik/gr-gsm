#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: GSM input adaptor
# Author: Piotr Krysik
# Description: Adaptor of input stream for the GSM receiver. Contains frequency ofset corrector doing also resampling to integer multiplies of GSM sample rate and LP filter filtering GSM channel.
# Generated: Wed Nov 19 08:23:52 2014
##################################################

from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes
from distutils.version import LooseVersion as version
import grgsm

class gsm_input(grgsm.hier_block):

    def __init__(self, ppm=0, osr=4, fc=940e6, samp_rate_in=1e6):
        grgsm.hier_block.__init__(
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
        # Variables
        ##################################################
        self.samp_rate_out = samp_rate_out = 1625000.0/6.0*osr

        ##################################################
        # Blocks
        ##################################################
        self.message_port_register_hier_in("ppm_in")

        self.low_pass_filter_0_0 = filter.fir_filter_ccf(1, firdes.low_pass(
        	1, samp_rate_out, 125e3, 5e3, firdes.WIN_HAMMING, 6.76))
        self.gsm_clock_offset_corrector_0 = grgsm.clock_offset_corrector(
            fc=fc,
            ppm=ppm,
            samp_rate_in=samp_rate_in,
        )
        self.fractional_resampler_xx_0 = filter.fractional_resampler_cc(0, samp_rate_in/samp_rate_out)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.low_pass_filter_0_0, 0), (self, 0))
        self.connect((self.fractional_resampler_xx_0, 0), (self.low_pass_filter_0_0, 0))
        self.connect((self.gsm_clock_offset_corrector_0, 0), (self.fractional_resampler_xx_0, 0))
        self.connect((self, 0), (self.gsm_clock_offset_corrector_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self, "ppm_in", self.gsm_clock_offset_corrector_0, "ppm_in")


    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.gsm_clock_offset_corrector_0.set_ppm(self.ppm)

    def get_osr(self):
        return self.osr

    def set_osr(self, osr):
        self.osr = osr
        self.set_samp_rate_out(1625000.0/6.0*self.osr)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.gsm_clock_offset_corrector_0.set_fc(self.fc)

    def get_samp_rate_in(self):
        return self.samp_rate_in

    def set_samp_rate_in(self, samp_rate_in):
        self.samp_rate_in = samp_rate_in
        self.fractional_resampler_xx_0.set_resamp_ratio(self.samp_rate_in/self.samp_rate_out)
        self.gsm_clock_offset_corrector_0.set_samp_rate_in(self.samp_rate_in)

    def get_samp_rate_out(self):
        return self.samp_rate_out

    def set_samp_rate_out(self, samp_rate_out):
        self.samp_rate_out = samp_rate_out
        self.low_pass_filter_0_0.set_taps(firdes.low_pass(1, self.samp_rate_out, 125e3, 5e3, firdes.WIN_HAMMING, 6.76))
        self.fractional_resampler_xx_0.set_resamp_ratio(self.samp_rate_in/self.samp_rate_out)

