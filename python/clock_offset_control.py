#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2014 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from numpy import *
from gnuradio import gr
import pmt

class clock_offset_control(gr.basic_block):
    """
    docstring for block clock_offset_control
    """
    def __init__(self, fc, samp_rate):
        gr.basic_block.__init__(self,
            name="clock_offset_control",
            in_sig=[],
            out_sig=[])
        self.fc = fc
        self.samp_rate = samp_rate
        self.message_port_register_in(pmt.intern("measurements"))
        self.set_msg_handler(pmt.intern("measurements"), self.process_measurement)
        self.message_port_register_out(pmt.intern("ppm"))
        self.alfa = 0.6
        self.ppm_estimate = -1e6
        self.first_measurement = True
        self.counter = 0
        
    def process_measurement(self,msg):
        if pmt.is_tuple(msg):
            key = pmt.symbol_to_string(pmt.tuple_ref(msg,0))
            if key == "freq_offset":
                freq_offset = pmt.to_double(pmt.tuple_ref(msg,1))
                ppm = -freq_offset/self.fc*1.0e6
                state = pmt.symbol_to_string(pmt.tuple_ref(msg,2))
                
                if abs(ppm) > 50:
                    ppm = 0
                    self.reset()
                    
                if state == "fcch_search":
                    msg_ppm = pmt.from_double(ppm)
                    self.message_port_pub(pmt.intern("ppm"), msg_ppm)
                
                if state == "synchronized":
                    if self.first_measurement:
                        self.ppm_estimate = ppm
                        self.first_measurement = False
                    else:
                        self.ppm_estimate = (1-self.alfa)*self.ppm_estimate+self.alfa*ppm
                    
                    if self.counter == 5:
                        self.counter = 0
                        msg_ppm = pmt.from_double(ppm)
                        self.message_port_pub(pmt.intern("ppm"), msg_ppm)
                    else:
                        self.counter=self.counter+1
                    
                if state == "sync_loss":
                    self.reset()
                    msg_ppm = pmt.from_double(0.0)
                    self.message_port_pub(pmt.intern("ppm"), msg_ppm)
    def reset(self):
        self.ppm_estimate = -1e6
        self.counter = 0
        self.first_measurement = True
