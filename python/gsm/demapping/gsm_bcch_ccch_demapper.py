#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#
# Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
#

##################################################
# GNU Radio Python Flow Graph
# Title: BCCH + CCCH demapper
# Author: Piotr Krysik
# Description: Demapper for BCCH + CCCH control channels. This corresponds to channel combination iv specified in GSM 05.02, section 6.4
# Generated: Mon May 23 09:32:44 2016
##################################################

from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio import gsm


class gsm_bcch_ccch_demapper(gr.hier_block2):

    def __init__(self, timeslot_nr=0):
        gr.hier_block2.__init__(
            self, "BCCH + CCCH demapper",
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

        # 3GPP TS 45.002 version 15.1.0 Release 15
        # Table 3 : Mapping of logical channels onto physical channels (see subclauses 6.3, 6.4, 6.5)
        # BCCH Norm D 0,2,4,6 C0 NB 51 B(2..5)
        # RACH U 0,2,4,6 C0 AB, Extended AB2 51 B0(0),B1(1)..B50(50)
        # Figure 8a: TDMA frame mapping for FCCH + SCH + BCCH + CCCH
        self.gsm_universal_ctrl_chans_demapper_0 = gsm.universal_ctrl_chans_demapper(
                timeslot_nr, ([ #downlink
                    0,0,
                    2,2,2,2,
                    6,6,6,6,
                    0,0,
                    12,12,12,12,
                    16,16,16,16,
                    0,0,
                    22,22,22,22,
                    26,26,26,26,
                    0,0,
                    32,32,32,32,
                    36,36,36,36,
                    0,0,
                    42,42,42,42,
                    46,46,46,46,
                    0,
                ]), ([
                    0,0,
                    1,1,1,1,
                    2,2,2,2,
                    0,0,
                    2,2,2,2,
                    2,2,2,2,
                    0,0,
                    2,2,2,2,
                    2,2,2,2,
                    0,0,
                    2,2,2,2,
                    2,2,2,2,
                    0,0,
                    2,2,2,2,
                    2,2,2,2,
                    0,
                ]), ([
                    0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,
                    1,1,1,1,
                    2,2,2,2,
                    0,0,
                    3,3,3,3,
                    4,4,4,4,
                    0,0,
                    5,5,5,5,
                    6,6,6,6,
                    0,0,
                    7,7,7,7,
                    8,8,8,8,
                    0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    1,1,1,1,
                    2,2,2,2,
                    0,0,
                    3,3,3,3,
                    4,4,4,4,
                    0,0,
                    5,5,5,5,
                    6,6,6,6,
                    0,0,
                    7,7,7,7,
                    8,8,8,8,
                    0,
                ]), ([ #uplink
                    0,1,2,3,
                    4,5,6,7,
                    8,9,10,11,
                    12,13,14,15,
                    16,17,18,19,
                    20,21,22,23,
                    24,25,26,27,
                    28,29,30,31,
                    32,33,34,35,
                    36,37,38,39,
                    40,41,42,43,
                    44,45,46,47,
                    48,49,50,
                ]), ([
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,
                ]), ([
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,
                ]))

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.gsm_universal_ctrl_chans_demapper_0, 'bursts'), (self, 'bursts'))    
        self.msg_connect((self, 'bursts'), (self.gsm_universal_ctrl_chans_demapper_0, 'bursts'))    

    def get_timeslot_nr(self):
        return self.timeslot_nr

    def set_timeslot_nr(self, timeslot_nr):
        self.timeslot_nr = timeslot_nr
