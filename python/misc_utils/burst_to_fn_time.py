"""
Embedded Python Blocks:

Each this file is saved, GRC will instantiate the first class it finds to get
ports and parameters of your block. The arguments to __init__  will be the
parameters. All of them are required to have default values!
"""
import numpy as np
from gnuradio import gr
import pmt

class burst_to_fn_time(gr.basic_block):
    def __init__(self):  # only default arguments here
        gr.basic_block.__init__(
            self,
            name='Burst to fn_time',
            in_sig=[],
            out_sig=[]
        )
        self.message_port_register_in(pmt.intern("bursts_in"))
        self.message_port_register_out(pmt.intern("fn_time_out"))
        self.set_msg_handler(pmt.intern("bursts_in"), self.convert)
    
    def convert(self, msg):
        fn_time = pmt.dict_ref(pmt.car(msg),pmt.intern("fn_time"),pmt.PMT_NIL)
        fn_time_msg = pmt.dict_add(pmt.make_dict(), pmt.intern("fn_time"), fn_time)
        if pmt.to_python(fn_time) is not None:
            self.message_port_pub(pmt.intern("fn_time_out"), fn_time_msg)
