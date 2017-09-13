#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @file
# @author Piotr Krysik <ptrkrysik@gmail.com>
# @section LICENSE
# 
# Gr-gsm is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# Gr-gsm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with gr-gsm; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 
# 

import numpy
from gnuradio import gr

class txtime_bursts_tagger(gr.basic_block):
    """
    A block that adds txtime metadata to a burst
    """
    def __init__(self):
        gr.basic_block.__init__(self,
            name="txtime_bursts_tagger",
            in_sig=[],
            out_sig=[])
        self.message_port_register_in(pmt.intern("fn_time"))
        self.message_port_register_in(pmt.intern("bursts"))
        self.message_port_register_out(pmt.intern("bursts"))
    
        self.set_msg_handler(pmt.intern("fn_time"), self.set_fn_time_reference)
        self.set_msg_handler(pmt.intern("bursts"), self.set_txtime_of_burst)
        
    def set_fn_time_reference(self, msg):
        pass
        
    def set_txtime_of_burst()
        pass
