#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# @file
# @author (C) 2016 by Piotr Krysik <ptrkrysik@gmail.com>
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
##################################################
# GNU Radio Python Flow Graph
# Title: BCCH + CCCH + SDCCH/4 demapper
# Author: Piotr Krysik
# Description: Demapper for BCCH + CCCH + SDCCH/4 + SACCH/C4 control channels. This corresponds to channel combination v specified in GSM 05.02, section 6.4
# Generated: Mon May 23 09:32:46 2016
##################################################

from gnuradio import gr
from gnuradio.filter import firdes
import grgsm


class gsm_bcch_ccch_sdcch4_demapper(grgsm.hier_block):

    def __init__(self, timeslot_nr=0):
        grgsm.hier_block.__init__(
            self, "BCCH + CCCH + SDCCH/4 demapper",
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
        self.gsm_universal_ctrl_chans_demapper_0 = grgsm.universal_ctrl_chans_demapper(timeslot_nr, ([0,0,2,2,2,2,6,6,6,6,0,0,12,12,12,12,16,16,16,16,0,0,22,22,22,22,26,26,26,26,0,0,32,32,32,32,36,36,36,36,0,0,42,42,42,42,46,46,46,46,0]), ([0,0,1,1,1,1,2,2,2,2,0,0,2,2,2,2,2,2,2,2,0,0,7,7,7,7,7,7,7,7,0,0,7,7,7,7,7,7,7,7,0,0,135,135,135,135,135,135,135,135,0]), ([0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,0,0,0,0,0,0,1,1,1,1,0,0,2,2,2,2,3,3,3,3,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,0,0,0,0,0,0,1,1,1,1,0,0,2,2,2,2,3,3,3,3,0,0,2,2,2,2,3,3,3,3,0]), ([0,0,0,0,0,0,6,6,6,6,10,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,37,37,37,37,41,41,41,41,0,0,47,47,47,47]), ([7,7,7,7,0,0,135,135,135,135,135,135,135,135,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,7,7,7,7,7,0,0,7,7,7,7]), ([0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,0,0,3,3,3,3,0,0,0,0,0,0,2,2,2,2,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,0,0,3,3,3,3]))

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.gsm_universal_ctrl_chans_demapper_0, 'bursts'), (self, 'bursts'))    
        self.msg_connect((self, 'bursts'), (self.gsm_universal_ctrl_chans_demapper_0, 'bursts'))    

    def get_timeslot_nr(self):
        return self.timeslot_nr

    def set_timeslot_nr(self, timeslot_nr):
        self.timeslot_nr = timeslot_nr
