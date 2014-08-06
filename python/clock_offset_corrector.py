#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Clock offset corrector
# Author: Piotr Krysik
# Generated: Wed Aug  6 13:47:38 2014
##################################################

from gnuradio import blocks
from gnuradio import filter
from gnuradio import gr
from gnuradio.filter import firdes
import gsm
import math

class clock_offset_corrector(gr.hier_block2):

    def __init__(self, ppm=0, samp_rate=1625000.0/6.0*4.0, fc=936.6e6):
        gr.hier_block2.__init__(
            self, "Clock offset corrector",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.ppm = ppm
        self.samp_rate = samp_rate
        self.fc = fc

        ##################################################
        # Blocks
        ##################################################
        self.ppm_msg = None;self.message_port_register_hier_out("ppm_msg")
        self.gsm_controlled_rotator_cc_0 = gsm.controlled_rotator_cc(0,samp_rate)
        self.gsm_controlled_const_source_f_0 = gsm.controlled_const_source_f(ppm)
        self.fractional_resampler_xx_0 = filter.fractional_resampler_cc(0, 1)
        self.blocks_multiply_const_vxx_0_0 = blocks.multiply_const_vff((1.0/1.0e6, ))
        self.blocks_multiply_const_vxx_0 = blocks.multiply_const_vff((fc/samp_rate*(2*math.pi), ))
        self.blocks_add_const_vxx_0 = blocks.add_const_vff((1, ))

        ##################################################
        # Connections
        ##################################################
        self.connect((self.gsm_controlled_const_source_f_0, 0), (self.blocks_multiply_const_vxx_0_0, 0))
        self.connect((self, 0), (self.gsm_controlled_rotator_cc_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.blocks_multiply_const_vxx_0, 0))
        self.connect((self.blocks_add_const_vxx_0, 0), (self.fractional_resampler_xx_0, 1))
        self.connect((self.gsm_controlled_rotator_cc_0, 0), (self.fractional_resampler_xx_0, 0))
        self.connect((self.blocks_multiply_const_vxx_0, 0), (self.gsm_controlled_rotator_cc_0, 1))
        self.connect((self.blocks_multiply_const_vxx_0_0, 0), (self.blocks_add_const_vxx_0, 0))
        self.connect((self.fractional_resampler_xx_0, 0), (self, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self, "ppm_msg", self.gsm_controlled_const_source_f_0, "constant_msg")


    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.gsm_controlled_const_source_f_0.set_constant(self.ppm)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.blocks_multiply_const_vxx_0.set_k((self.fc/self.samp_rate*(2*math.pi), ))
        self.gsm_controlled_rotator_cc_0.set_samp_rate(self.samp_rate)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.blocks_multiply_const_vxx_0.set_k((self.fc/self.samp_rate*(2*math.pi), ))

