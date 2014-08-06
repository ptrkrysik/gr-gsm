#!/usr/bin/env python

import weakref
import gsm
from gnuradio.eng_option import eng_option
from gnuradio import gr, gru, blocks
from gnuradio import filter


class tuner(gr.feval_dd):
    def __init__(self, top_block):
        gr.feval_dd.__init__(self)
        self.top_block = top_block
    def eval(self, freq_offet):
        self.top_block.set_center_frequency(freq_offet)
        return freq_offet

class receiver_hier(gr.hier_block2):
    def __init__(self, input_rate, osr=4, arfcn=0):
        gr.hier_block2.__init__(self, 
                                "receiver_hier",
                                gr.io_signature(1, 1, gr.sizeof_gr_complex),
                                gr.io_signature(1, 1, 142*gr.sizeof_float))
        #set rates
        gsm_symb_rate = 1625000/6.0

        self.message_port_register_hier_in("bursts")
        self.message_port_register_hier_in("measurements")

        self.input_rate = input_rate
        self.osr = osr
        self.arfcn = arfcn
        self.sps = input_rate / gsm_symb_rate / osr

        #create callbacks
        self.tuner_callback = tuner(self)
        #create accompaning blocks
        self.filtr = self._set_filter()
        self.interpolator = self._set_interpolator()
        self.receiver = self._set_receiver()
        self.connect(self, self.filtr,  self.interpolator, self.receiver,  self)
#        self.connect(self, self.interpolator, self.receiver,  self)
        self.msg_connect(self.receiver, "bursts", weakref.proxy(self), "bursts")
        self.msg_connect(self.receiver, "measurements", weakref.proxy(self), "measurements")

    def _set_filter(self):
        filter_cutoff   = 125e3	
        filter_t_width  = 10e3
        offset = 0

        filter_taps    = filter.firdes.low_pass(1.0, self.input_rate, filter_cutoff, filter_t_width, filter.firdes.WIN_HAMMING)
        filtr          = filter.freq_xlating_fir_filter_ccf(1, filter_taps, offset, self.input_rate)
        return filtr
    
    def _set_interpolator(self):
        interpolator = filter.fractional_resampler_cc(0, self.sps) 
        return interpolator
    
    def _set_receiver(self):
        receiver = gsm.receiver(self.tuner_callback, self.osr, self.arfcn)
        return receiver

    def set_center_frequency(self, center_freq):
        self.filtr.set_center_freq(center_freq)

    def set_timing(self, timing_offset):
        pass
        
    def set_arfcn(self,arfcn):
        self.receiver.set_arfcn(arfcn)
        
    def reset(self):
        self.receiver.reset()
        
