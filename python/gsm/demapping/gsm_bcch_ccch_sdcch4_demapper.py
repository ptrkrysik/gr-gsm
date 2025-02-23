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
# Title: BCCH + CCCH + SDCCH/4 demapper
# Author: Piotr Krysik
# Description: Demapper for BCCH + CCCH + SDCCH/4 + SACCH/C4 control channels. This corresponds to channel combination v specified in GSM 05.02, section 6.4
# Generated: Mon May 23 09:32:46 2016
##################################################

from gnuradio import gr
from gnuradio.filter import firdes
from gnuradio import gsm


class gsm_bcch_ccch_sdcch4_demapper(gr.hier_block2):

    def __init__(self, timeslot_nr=0):
        gr.hier_block2.__init__(
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

        # 3GPP TS 45.002 version 15.1.0 Release 15
        # Table 3 : Mapping of logical channels onto physical channels (see subclauses 6.3, 6.4, 6.5)
        #
        # BCCH Norm D 0,2,4,6 C0 NB 51 B(2..5)
        # SDCCH/4 0 D 0 C0 NB1 51 B(22..25)
        #           U B(37..40)
        #         1 D B(26..29)
        #           U B(41..44)
        #         2 D B(32..35)
        #           U B(47..50)
        #         3 D B(36..39)
        #           U B(0..3)
        # SACCH/C4 0 D 0 C0 NB3 102 B(42..45)
        #            U B(57..60)
        #          1 D B(46..49)
        #            U B(61..64)
        #          2 D B(93..96)
        #            U B(6..9)
        #          3 D B(97..100)
        #            U B(10..13)
        #
        # Figure 8b: TDMA frame mapping for FCCH + SCH + BCCH + CCCH + SDCCH/4(0...3) + SACCH/4(0...3)
        #
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
                    7,7,7,7,
                    7,7,7,7,
                    0,0,
                    7,7,7,7,
                    7,7,7,7,
                    0,0,
                    135,135,135,135,
                    135,135,135,135,
                    0,
                ]), ([
                    0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,
                    1,1,1,1,
                    2,2,2,2,
                    0,0,
                    0,0,0,0,
                    1,1,1,1,
                    0,0,
                    2,2,2,2,
                    3,3,3,3,
                    0,0,
                    0,0,0,0,
                    1,1,1,1,
                    0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,
                    0,
                    1,1,1,1,
                    2,2,2,2,
                    0,0,
                    0,0,0,0,
                    1,1,1,1,
                    0,0,
                    2,2,2,2,
                    3,3,3,3,
                    0,0,
                    2,2,2,2,
                    3,3,3,3,
                    0,
                ]), ([  #uplink
                    0,0,0,0,
                    4,5,
                    6,6,6,6,
                    10,10,10,10,
                    14,15,16,17,
                    18,19,20,21,
                    22,23,24,25,
                    26,27,28,29,
                    30,31,32,33,
                    34,35,36,
                    37,37,37,37,
                    41,41,41,41,
                    45,46,
                    47,47,47,47,
                ]), ([
                    7,7,7,7,
                    3,3,
                    135,135,135,135,
                    135,135,135,135,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,3,3,
                    3,3,
                    3,
                    7,7,7,7,
                    7,7,7,7,
                    3,3,
                    7,7,7,7,
                ]), ([
                    3,3,3,3,
                    0,0,
                    2,2,2,2,
                    3,3,3,3,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,
                    0,
                    0,0,0,0,
                    1,1,1,1,
                    0,0,
                    2,2,2,2,
                    3,3,3,3,
                    0,0,
                    0,0,0,0,
                    1,1,1,1,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,0,0,
                    0,0,
                    0,
                    0,0,0,0,
                    1,1,1,1,
                    0,0,
                    2,2,2,2,
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
