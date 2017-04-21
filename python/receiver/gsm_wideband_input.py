#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: GSM wideband input adaptor
# Author: Piotr Krysik
# Co-author: Pieter Robyns
# Description: Adaptor of input stream for the GSM receiver. Contains frequency ofset corrector doing also resampling to integer multiplies of GSM sample rate and LP filter filtering GSM channel.
##################################################

from gnuradio import filter
from gnuradio import gr
from gnuradio import eng_notation
from gnuradio.filter import firdes
import grgsm.arfcn as arfcn
import grgsm

class gsm_wideband_input(grgsm.hier_block):
    def __init__(self, ppm=0, osr=4, fc=925.2e6, samp_rate_in=20e6, ca=[], band='P-GSM'):
        self.num_streams = len(ca)
        grgsm.hier_block.__init__(
            self, "GSM wideband input adaptor",
            gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
            gr.io_signature(self.num_streams, self.num_streams, gr.sizeof_gr_complex*1),
        )

        ##################################################
        # Parameters
        ##################################################
        self.ppm = ppm
        self.osr = osr
        self.fc = fc
        self.samp_rate_in = samp_rate_in
        self.ca = ca
        self.blocks_fir_filters = {}
        self.blocks_resamplers = {}
        self.blocks_ocs = {}
        self.band = band

        ##################################################
        # Variables
        ##################################################
        self.samp_rate_out = samp_rate_out = 1625000.0/6.0*osr

        ##################################################
        # Blocks
        ##################################################
        self.message_port_register_hier_in("ctrl_in")
        #self.lpf = firdes.low_pass(1, samp_rate_out, 125e3, 5e3, firdes.WIN_HAMMING, 6.76)
        self.lpf = firdes.low_pass(1, samp_rate_in, 125e3, 75e3, firdes.WIN_HAMMING, 6.76)
        self.gsm_clock_offset_corrector_0 = grgsm.clock_offset_corrector_tagged(
            fc=fc,
            samp_rate_in=samp_rate_in,
            ppm=ppm,
            osr=osr
        )

        center_arfcn = arfcn.downlink2arfcn(fc, band)
        fc_str = eng_notation.num_to_str(fc)
        print("Extracting channels %s, given center frequency at %sHz (ARFCN %d)" % (str(ca), fc_str, center_arfcn))

        self.connect((self, 0), (self.gsm_clock_offset_corrector_0, 0))

        output_port = 0
        for channel in ca:
            channel_freq = arfcn.arfcn2downlink(channel, band)
            if channel_freq is None:
                print("Warning: invalid ARFCN %d for band %s" % (channel, band))
                continue
            freq_diff = channel_freq - fc
            freq_diff_str = "+" if 0 <= freq_diff else ""
            freq_diff_str += eng_notation.num_to_str(freq_diff)
            print("ARFCN %d is at %sHz %sHz" % (channel, fc_str, freq_diff_str))

            self.blocks_resamplers[channel] = filter.fractional_resampler_cc(0, samp_rate_in/samp_rate_out)
            self.blocks_fir_filters[channel] = filter.freq_xlating_fir_filter_ccc(1, self.lpf, freq_diff, samp_rate_in)
            self.connect((self.gsm_clock_offset_corrector_0, 0), (self.blocks_fir_filters[channel], 0))
            self.connect((self.blocks_fir_filters[channel], 0), (self.blocks_resamplers[channel], 0))
            self.connect((self.blocks_resamplers[channel], 0), (self, output_port))
            output_port += 1

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self, "ctrl_in", self.gsm_clock_offset_corrector_0, "ctrl")

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.gsm_clock_offset_corrector_0.set_ppm(self.ppm)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.gsm_clock_offset_corrector_0.set_fc(self.fc)

    def get_osr(self):
        return self.osr

    def set_osr(self, osr):
        self.osr = osr
        self.set_samp_rate_out(1625000.0/6.0*self.osr)

    def get_samp_rate_in(self):
        return self.samp_rate_in

    def set_samp_rate_in(self, samp_rate_in):
        self.samp_rate_in = samp_rate_in
        for channel in self.blocks_resamplers:
            self.blocks_resamplers[channel].set_resamp_ratio(self.samp_rate_in / self.samp_rate_out)
        self.gsm_clock_offset_corrector_0.set_samp_rate_in(self.samp_rate_in)

    def get_samp_rate_out(self):
        return self.samp_rate_out

    def set_samp_rate_out(self, samp_rate_out):
        self.samp_rate_out = samp_rate_out
        for channel in self.blocks_resamplers:
            self.blocks_resamplers[channel].set_resamp_ratio(self.samp_rate_in / self.samp_rate_out)
