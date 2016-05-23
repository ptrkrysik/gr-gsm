# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: SDCCH/8 demapper
# Author: Piotr Krysik
# Description: Demapper for SDCCH/8 + SACCH/C8 control channels. This corresponds to channel combination vii specified in GSM 05.02, section 6.4
# Generated: Mon May 23 09:32:48 2016
##################################################

from gnuradio import gr
from gnuradio.filter import firdes
import grgsm


class gsm_sdcch8_demapper(grgsm.hier_block):

    def __init__(self, timeslot_nr=1):
        grgsm.hier_block.__init__(
            self, "SDCCH/8 demapper",
            gr.io_signature(0, 0, 0),
            gr.io_signature(0, 0, 0),
        )
        self.message_port_register_hier_in("bursts")
        self.message_port_register_hier_out("bursts")

        ##################################################
        # Parameters
        ##################################################
        self.timeslot_nr = timeslot_nr

        ##################################################
        # Blocks
        ##################################################
        self.gsm_universal_ctrl_chans_demapper_0 = grgsm.universal_ctrl_chans_demapper(timeslot_nr, ([0,0,0,0,4,4,4,4,8,8,8,8,12,12,12,12,16,16,16,16,20,20,20,20,24,24,24,24,28,28,28,28,32,32,32,32,36,36,36,36,40,40,40,40,44,44,44,44,0,0,0]), ([8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,136,136,136,136,136,136,136,136,136,136,136,136,136,136,136,136,0,0,0]), ([0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,0,0,0]), ([0,0,0,0,4,4,4,4,8,8,8,8,0,0,0,15,15,15,15,19,19,19,19,23,23,23,23,27,27,27,27,31,31,31,31,35,35,35,35,39,39,39,39,43,43,43,43,47,47,47,47]), ([136,136,136,136,136,136,136,136,136,136,136,136,0,0,0,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,136,136,136,136]), ([1,1,1,1,2,2,2,2,3,3,3,3,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,0,0,0,0,5,5,5,5,6,6,6,6,7,7,7,7,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,4,4,4,4]))

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.gsm_universal_ctrl_chans_demapper_0, 'bursts'), (self, 'bursts'))    
        self.msg_connect((self, 'bursts'), (self.gsm_universal_ctrl_chans_demapper_0, 'bursts'))    

    def get_timeslot_nr(self):
        return self.timeslot_nr

    def set_timeslot_nr(self, timeslot_nr):
        self.timeslot_nr = timeslot_nr
