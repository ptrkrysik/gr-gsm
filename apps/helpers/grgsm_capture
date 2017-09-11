#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @file
# @author (C) 2015 by Roman Khassraf <rkhassraf@gmail.com>
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

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from math import pi
from optparse import OptionParser

import grgsm
import osmosdr
import pmt
import signal
import sys


class grgsm_capture(gr.top_block):

    def __init__(self, fc, gain, samp_rate, ppm, arfcn, cfile=None, burst_file=None, verbose=False, rec_length=None, args=""):

        gr.top_block.__init__(self, "Gr-gsm Capture")
                
        ##################################################
        # Parameters
        ##################################################
        self.fc = fc
        self.gain = gain
        self.samp_rate = samp_rate
        self.ppm = ppm
        self.arfcn = arfcn
        self.cfile = cfile
        self.burst_file = burst_file
        self.verbose = verbose
        self.shiftoff = shiftoff = 400e3
        self.rec_length = rec_length
        
        ##################################################
        # Processing Blocks
        ##################################################

        self.rtlsdr_source = osmosdr.source( args="numchan=" + str(1) + " " + args )
        self.rtlsdr_source.set_sample_rate(samp_rate)
        self.rtlsdr_source.set_center_freq(fc - shiftoff, 0)
        self.rtlsdr_source.set_freq_corr(ppm, 0)
        self.rtlsdr_source.set_dc_offset_mode(2, 0)
        self.rtlsdr_source.set_iq_balance_mode(2, 0)
        self.rtlsdr_source.set_gain_mode(True, 0)
        self.rtlsdr_source.set_gain(gain, 0)
        self.rtlsdr_source.set_if_gain(20, 0)
        self.rtlsdr_source.set_bb_gain(20, 0)
        self.rtlsdr_source.set_antenna("", 0)
        self.rtlsdr_source.set_bandwidth(250e3+abs(shiftoff), 0)
        self.blocks_rotator = blocks.rotator_cc(-2*pi*shiftoff/samp_rate)
        
        if self.rec_length is not None:
            self.blocks_head_0 = blocks.head(gr.sizeof_gr_complex, int(samp_rate*rec_length))
        
        if self.verbose or self.burst_file:
            self.gsm_receiver = grgsm.receiver(4, ([self.arfcn]), ([]))
            self.gsm_input = grgsm.gsm_input(
                ppm=0,
                osr=4,
                fc=fc,
                samp_rate_in=samp_rate,
            )
            self.gsm_clock_offset_control = grgsm.clock_offset_control(fc-shiftoff, samp_rate, osr=4)

        if self.burst_file:
            self.gsm_burst_file_sink = grgsm.burst_file_sink(self.burst_file)

        if self.cfile:
            self.blocks_file_sink = blocks.file_sink(gr.sizeof_gr_complex*1, self.cfile, False)
            self.blocks_file_sink.set_unbuffered(False)

        if self.verbose:
            self.gsm_bursts_printer_0 = grgsm.bursts_printer(pmt.intern(""),
                                                             False, False, False, False)

        ##################################################
        # Connections
        ##################################################

        if self.rec_length is not None: #if recording length is defined connect head block after the source
            self.connect((self.rtlsdr_source, 0), (self.blocks_head_0, 0))
            self.connect((self.blocks_head_0, 0), (self.blocks_rotator, 0))
        else:
            self.connect((self.rtlsdr_source, 0), (self.blocks_rotator, 0))

        if self.cfile:
            self.connect((self.blocks_rotator, 0), (self.blocks_file_sink, 0))

        if self.verbose or self.burst_file:
            self.connect((self.gsm_input, 0), (self.gsm_receiver, 0))
            self.connect((self.blocks_rotator, 0), (self.gsm_input, 0))
            self.msg_connect(self.gsm_clock_offset_control, "ctrl", self.gsm_input, "ctrl_in")
            self.msg_connect(self.gsm_receiver, "measurements", self.gsm_clock_offset_control, "measurements")

            if self.burst_file:
                self.msg_connect(self.gsm_receiver, "C0", self.gsm_burst_file_sink, "in")
            if self.verbose:
                self.msg_connect(self.gsm_receiver, "C0", self.gsm_bursts_printer_0, "bursts")

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        if self.verbose or self.burst_file:
            self.gsm_input.set_fc(self.fc)
            
    def get_arfcn(self):
        return self.arfcn

    def set_arfcn(self, arfcn):
        self.arfcn = arfcn
        if self.verbose or self.burst_file:
            self.gsm_receiver.set_cell_allocation([self.arfcn])
            new_freq = grgsm.arfcn.arfcn2downlink(self.arfcn)
            self.set_fc(new_freq)

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.rtlsdr_source.set_sample_rate(self.samp_rate)
        if self.verbose or self.burst_file:
            self.gsm_input.set_samp_rate_in(self.samp_rate)

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.set_ppm_slider(self.ppm)

    def get_rec_length(self):
        return self.rec_length

    def set_rec_length(self, rec_length):
        self.rec_length = rec_length
        self.blocks_head_0.set_length(int(self.samp_rate*self.rec_length))


if __name__ == '__main__':

    parser = OptionParser(option_class=eng_option, usage="%prog [options]",
                          description="RTL-SDR capturing app of gr-gsm.")

    parser.add_option("-f", "--fc", dest="fc", type="eng_float", 
                      help="Set frequency [default=%default]")
    
    parser.add_option("-a", "--arfcn", dest="arfcn", type="intx", 
                      help="Set ARFCN instead of frequency (for PCS1900 add 0x8000 (2**15) to the ARFCN number)")
    
    parser.add_option("-g", "--gain", dest="gain", type="eng_float", 
                      default=eng_notation.num_to_str(30),
                      help="Set gain [default=%default]")
    
    parser.add_option("-s", "--samp-rate", dest="samp_rate", type="eng_float", 
                      default=eng_notation.num_to_str(1000000),
                      help="Set samp_rate [default=%default]")
    
    parser.add_option("-p", "--ppm", dest="ppm", type="intx", default=0,
                      help="Set ppm [default=%default]")
    
    parser.add_option("-b", "--burst-file", dest="burst_file", 
                      help="File where the captured bursts are saved")
    
    parser.add_option("-c", "--cfile", dest="cfile", 
                      help="File where the captured data are saved")
    
    parser.add_option("", "--args", dest="args", type="string", default="",
        help="Set device arguments [default=%default]")

    parser.add_option("-v", "--verbose", action="store_true", 
                      help="If set, the captured bursts are printed to stdout")

    parser.add_option("-T", "--rec-length", dest="rec_length", type="eng_float",
        help="Set length of recording in seconds [default=%default]")

    (options, args) = parser.parse_args()
    
    if options.cfile is None and options.burst_file is None:
        parser.error("Please provide a cfile or a burst file (or both) to save the captured data\n")
        
    if (options.fc is None and options.arfcn is None) or (options.fc is not None and options.arfcn is not None):
        parser.error("You have to provide either a frequency or an ARFCN (but not both).\n")
        
    arfcn = 0
    fc = 939.4e6
    if options.arfcn:
        if not grgsm.arfcn.is_valid_arfcn(options.arfcn):
            parser.error("ARFCN is not valid\n")
        else:
            arfcn = options.arfcn
            fc = grgsm.arfcn.arfcn2downlink(arfcn)
    elif options.fc:
        fc = options.fc
        arfcn = grgsm.arfcn.downlink2arfcn(options.fc)
     
    tb = grgsm_capture(fc=fc, gain=options.gain, samp_rate=options.samp_rate,
                         ppm=options.ppm, arfcn=arfcn, cfile=options.cfile, 
                         burst_file=options.burst_file, verbose=options.verbose,
                         rec_length=options.rec_length, args=options.args)
    
    def signal_handler(signal, frame):
        tb.stop()
        tb.wait()        
        sys.exit(0)
        
    signal.signal(signal.SIGINT, signal_handler)

    tb.start()
    tb.wait()
