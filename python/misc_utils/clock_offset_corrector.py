#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Clock offset corrector
# Author: Piotr Krysik
# Generated: Thu Nov  6 10:22:24 2014
##################################################

from gnuradio import blocks
from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes
import gsm
import math

class clock_offset_corrector(gr.hier_block2):

    def __init__(self, fc=936.6e6, samp_rate_in=1625000.0/6.0*4.0, samp_rate_out=1625000.0/6.0*4.0, ppm=0):
        gr.hier_block2.__init__(
            self, "Clock offset corrector",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.fc = fc
        self.samp_rate_in = samp_rate_in
        self.samp_rate_out = samp_rate_out
        self.ppm = ppm

        ##################################################
        # Blocks
        ##################################################
        self.ppm_in = None;self.message_port_register_hier_out("ppm_in")
        self.gsm_controlled_rotator_cc_0 = gsm.controlled_rotator_cc(0,samp_rate_out)
        self.gsm_controlled_const_source_f_0 = gsm.controlled_const_source_f(ppm)
        self.fractional_resampler_xx_0_0 = filter.fractional_resampler_ff(0, samp_rate_in/samp_rate_out)
        self.fractional_resampler_xx_0 = filter.fractional_resampler_cc(0, samp_rate_in/samp_rate_out)
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vff((1.0e-6*samp_rate_in/samp_rate_out, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((fc/samp_rate_out*(2*math.pi)/1e6, ))
        self.blocks_add_const_vxx_0 = blocks.add_const_vff((samp_rate_in/samp_rate_out, ))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.gsm_controlled_const_source_f_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self, 0), (self.fractional_resampler_xx_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.fractional_resampler_xx_0_0, 0))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.fractional_resampler_xx_0_0, 1))
        self.connect((self.gsm_controlled_rotator_cc_0, 0), (self, 0))
        self.connect((self.fractional_resampler_xx_0, 0), (self.gsm_controlled_rotator_cc_0, 0))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.fractional_resampler_xx_0, 1))
        self.connect((self.gsm_controlled_const_source_f_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.blocks_add_const_vxx_0, 0))
        self.connect((self.fractional_resampler_xx_0_0, 0), (self.gsm_controlled_rotator_cc_0, 1))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self, "ppm_in", self.gsm_controlled_const_source_f_0, "constant_msg")


    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.blocks_multiply_const_vxx_0.set_k((self.fc/self.samp_rate_out*(2*math.pi)/1e6, ))

    def get_samp_rate_in(self):
        return self.samp_rate_in

    def set_samp_rate_in(self, samp_rate_in):
        self.samp_rate_in = samp_rate_in
        self.fractional_resampler_xx_0.set_resamp_ratio(self.samp_rate_in/self.samp_rate_out)
        self.fractional_resampler_xx_0_0.set_resamp_ratio(self.samp_rate_in/self.samp_rate_out)
        self.blocks_multiply_const_vxx_0_0.set_k((1.0e-6*self.samp_rate_in/self.samp_rate_out, ))
        self.blocks_add_const_vxx_0.set_k((self.samp_rate_in/self.samp_rate_out, ))

    def get_samp_rate_out(self):
        return self.samp_rate_out

    def set_samp_rate_out(self, samp_rate_out):
        self.samp_rate_out = samp_rate_out
        self.fractional_resampler_xx_0.set_resamp_ratio(self.samp_rate_in/self.samp_rate_out)
        self.gsm_controlled_rotator_cc_0.set_samp_rate(self.samp_rate_out)
        self.blocks_multiply_const_vxx_0.set_k((self.fc/self.samp_rate_out*(2*math.pi)/1e6, ))
        self.fractional_resampler_xx_0_0.set_resamp_ratio(self.samp_rate_in/self.samp_rate_out)
        self.blocks_multiply_const_vxx_0_0.set_k((1.0e-6*self.samp_rate_in/self.samp_rate_out, ))
        self.blocks_add_const_vxx_0.set_k((self.samp_rate_in/self.samp_rate_out, ))

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.gsm_controlled_const_source_f_0.set_constant(self.ppm)

