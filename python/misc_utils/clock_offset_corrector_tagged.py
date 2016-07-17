# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Clock offset corrector tagged
# Author: Piotr Krysik
# Description: Clock offset corrector with blocks that use tags to switch offsets
# Generated: Sun Jul 17 11:30:51 2016
##################################################

from gnuradio import gr
from gnuradio.filter import firdes
import grgsm
import math


class clock_offset_corrector_tagged(grgsm.hier_block):

    def __init__(self, fc=936.6e6, ppm=0, samp_rate_in=1625000.0/6.0*4.0):
        gr.hier_block2.__init__(
            self, "Clock offset corrector tagged",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
        )
        self.message_port_register_hier_in("ctrl")

        ##################################################
        # Parameters
        ##################################################
        self.fc = fc
        self.ppm = ppm
        self.samp_rate_in = samp_rate_in

        ##################################################
        # Variables
        ##################################################
        self.samp_rate_out = samp_rate_out = samp_rate_in

        ##################################################
        # Blocks
        ##################################################
        self.gsm_msg_to_tag_0 = grgsm.msg_to_tag()
        self.gsm_controlled_rotator_cc_0 = grgsm.controlled_rotator_cc(ppm/1.0e6*2*math.pi*fc/samp_rate_in,samp_rate_out)
        self.gsm_controlled_fractional_resampler_cc_0 = grgsm.controlled_fractional_resampler_cc(0, 1.0)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self, 'ctrl'), (self.gsm_msg_to_tag_0, 'msg'))    
        self.connect((self.gsm_controlled_fractional_resampler_cc_0, 0), (self.gsm_controlled_rotator_cc_0, 0))    
        self.connect((self.gsm_controlled_rotator_cc_0, 0), (self, 0))    
        self.connect((self.gsm_msg_to_tag_0, 0), (self.gsm_controlled_fractional_resampler_cc_0, 0))    
        self.connect((self, 0), (self.gsm_msg_to_tag_0, 0))    

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.gsm_controlled_rotator_cc_0.set_phase_inc(self.ppm/1.0e6*2*math.pi*self.fc/self.samp_rate_in)

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.gsm_controlled_rotator_cc_0.set_phase_inc(self.ppm/1.0e6*2*math.pi*self.fc/self.samp_rate_in)

    def get_samp_rate_in(self):
        return self.samp_rate_in

    def set_samp_rate_in(self, samp_rate_in):
        self.samp_rate_in = samp_rate_in
        self.set_samp_rate_out(self.samp_rate_in)
        self.gsm_controlled_rotator_cc_0.set_phase_inc(self.ppm/1.0e6*2*math.pi*self.fc/self.samp_rate_in)

    def get_samp_rate_out(self):
        return self.samp_rate_out

    def set_samp_rate_out(self, samp_rate_out):
        self.samp_rate_out = samp_rate_out
        self.gsm_controlled_rotator_cc_0.set_samp_rate(self.samp_rate_out)
