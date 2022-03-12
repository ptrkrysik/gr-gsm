#!/usr/bin/env python2
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Multi Arfcns GSM Receiver
# Author: Deligrim
# Copyright: Deligrim
# Description: Multi Arfcns GSM Receiver
# GNU Radio version: 3.8.2.0
from PyQt5 import Qt
from gnuradio import qtgui
import sip
from gnuradio.filter import firdes
from gnuradio import blocks
from gnuradio import gr
from gnuradio.filter import pfb
import gsm
import gsm.arfcn as arfcn
import math

class multiarfcns_receiver(gr.hier_block2, Qt.QWidget):
    def __init__(self, arfcns=[725,661], center_freq=935e6, osr=4, tseq=1, wide_samp_rate=2e6, gui=False):
        gr.hier_block2.__init__(
            self, "Multi Arfcns GSM Receiver",
                gr.io_signature(1, 1, gr.sizeof_gr_complex*1),
                gr.io_signaturev(len(arfcns), len(arfcns), list(map(lambda x: gr.sizeof_gr_complex*1, arfcns))),
        )
        self.message_port_register_hier_out("out_c0")
        self.message_port_register_hier_out("out_cx")


        Qt.QWidget.__init__(self)
        self.top_layout = Qt.QVBoxLayout()
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)
        self.setLayout(self.top_layout)
        
        ##################################################
        # Parameters
        ##################################################
        self.arfcns = arfcns
        self.center_freq = center_freq
        self.osr = osr
        self.tseq = tseq
        self.wide_samp_rate = wide_samp_rate
        self.gui = gui
        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 1e6
        self.fcs = fcs = list(map(lambda x: arfcn.arfcn2downlink(x), arfcns))

        ##################################################
        # Blocks
        ##################################################
        self.gsm_receiver = gsm.receiver(osr, arfcns, [tseq], False)
        self.gsm_clock_offset_control = gsm.clock_offset_control(fcs[0], samp_rate, osr)
        self.pfb_arb_resamplers_xxx = []
        self.gsm_inputs = []
        self.blocks_rotators_cc = []
        self.qtgui_waterfall_sinks_x_win = []
        
        
        for i in range(len(self.fcs)):   
            resampler = pfb.arb_resampler_ccf(
                samp_rate/wide_samp_rate,
                taps=None,
                flt_size=32)
            resampler.declare_sample_delay(0)
            self.pfb_arb_resamplers_xxx.append(resampler)
            
            gsm_input = gsm.gsm_input(
                ppm=0,
                osr=osr,
                fc=fcs[i],
                samp_rate_in=samp_rate,
            )
            self.gsm_inputs.append(gsm_input)
            
            blocks_rotator_cc = blocks.rotator_cc(-2*math.pi*(fcs[i] - center_freq)/wide_samp_rate)
            self.blocks_rotators_cc.append(blocks_rotator_cc)
            if gui:
                qtgui_waterfall_sink_x = qtgui.waterfall_sink_c(
                    512, #size
                    firdes.WIN_BLACKMAN_hARRIS, #wintype
                    fcs[i], #fc
                    samp_rate, #bw
                    "", #name
                    1 #number of inputs
                )
                
                qtgui_waterfall_sink_x.set_update_time(1/20)
                qtgui_waterfall_sink_x.enable_grid(True)
                qtgui_waterfall_sink_x.enable_axis_labels(True)

                labels = ['', '', '', '', '',
                        '', '', '', '', '']
                colors = [0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0]
                alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                        1.0, 1.0, 1.0, 1.0, 1.0]

                for y in range(1):
                    if len(labels[y]) == 0:
                        qtgui_waterfall_sink_x.set_line_label(y, "Data {0}".format(y))
                    else:
                        qtgui_waterfall_sink_x.set_line_label(y, labels[y])
                    qtgui_waterfall_sink_x.set_color_map(y, colors[y])
                    qtgui_waterfall_sink_x.set_line_alpha(y, alphas[y])

                qtgui_waterfall_sink_x.set_intensity_range(-90, -30)

                _qtgui_waterfall_sink_x_win = sip.wrapinstance(qtgui_waterfall_sink_x.pyqwidget(), Qt.QWidget)
                self.qtgui_waterfall_sinks_x_win.append(_qtgui_waterfall_sink_x_win)
                
                self.top_grid_layout.addWidget(_qtgui_waterfall_sink_x_win, int(i/3), i%3, 1, 1)
                self.connect((gsm_input, 0), (qtgui_waterfall_sink_x, 0))
            
            ##################################################
            # Connections
            ##################################################
            self.msg_connect((self.gsm_clock_offset_control, 'ctrl'), (gsm_input, 'ctrl_in'))   
            self.connect((blocks_rotator_cc, 0), (resampler, 0))
            self.connect((gsm_input, 0), (self.gsm_receiver, i))
            self.connect((gsm_input, 0), (self, i))
            self.connect((self, 0), (blocks_rotator_cc, 0))
            self.connect((resampler, 0), (gsm_input, 0))
        
        self.msg_connect((self.gsm_receiver, 'measurements'), (self.gsm_clock_offset_control, 'measurements'))
        self.msg_connect((self.gsm_receiver, 'C0'), (self, 'out_c0'))
        self.msg_connect((self.gsm_receiver, 'CX'), (self, 'out_cx'))


    def get_arfcns(self):
        return self.arfcns

    # def set_arfcns(self, arfcns):
    #     self.arfcns = arfcns
    #     self.set_fcs(list(map(lambda x: arfcn.arfcn2downlink(x), self.arfcns)))

    def get_center_freq(self):
        return self.center_freq

    def set_center_freq(self, center_freq):
        self.center_freq = center_freq
        for i in range(len(self.fcs)):
            self.blocks_rotators_cc[i].set_phase_inc(-2*math.pi*(self.fcs[i] - self.center_freq)/self.wide_samp_rate)

    def get_osr(self):
        return self.osr

    def set_osr(self, osr):
        self.osr = osr
        for i in range(len(self.fcs)):
            self.gsm_inputs[i].set_osr(self.osr)

    def get_tseq(self):
        return self.tseq

    def set_tseq(self, tseq):
        self.tseq = tseq

    def get_wide_samp_rate(self):
        return self.wide_samp_rate

    def set_wide_samp_rate(self, wide_samp_rate):
        self.wide_samp_rate = wide_samp_rate
        for i in range(len(self.fcs)):
            self.blocks_rotators_cc[i].set_phase_inc(-2*math.pi*(self.fcs[i] - self.center_freq)/self.wide_samp_rate)
            self.pfb_arb_resamplers_xxx[i].set_rate(self.samp_rate/self.wide_samp_rate)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        for i in range(len(self.fcs)):
            self.gsm_inputs[i].set_samp_rate_in(self.samp_rate)
            self.pfb_arb_resamplers_xxx[i].set_rate(self.samp_rate/self.wide_samp_rate)

    def get_fcs(self):
        return self.fcs
    
    def get_gui(self):
        return self.gui

    # def set_fcs(self, fcs):
    #     self.fcs = fcs
    #     self.blocks_rotator_cc_0.set_phase_inc(-2*math.pi*(self.fcs[0] - self.center_freq)/self.wide_samp_rate)
    #     self.blocks_rotator_cc_1.set_phase_inc(-2*math.pi*(self.fcs[1] - self.center_freq)/self.wide_samp_rate)
    #     self.gsm_clock_offset_control_0.set_fc(self.fcs[0])
    #     self.gsm_input_0.set_fc(self.fcs[0])
    #     self.gsm_input_1.set_fc(self.fcs[1])


