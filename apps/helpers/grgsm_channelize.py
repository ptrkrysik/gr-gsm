#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# @file
# @author Pieter Robyns <pieter.robyns@uhasselt.be>
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

##################################################
# gr-gsm channelizer
#
# Standalone application to channelize a wideband
# GSM capture file into multiple seperate capture
# files for the specified ARFCNs.
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from argparse import ArgumentParser, ArgumentTypeError, RawDescriptionHelpFormatter
import grgsm.arfcn as arfcn
import os

EXTRA_HELP = """
Example usage:
gsm_channelize.py -f my_wideband_capture.cfile -c 925.2e6 990 991 992 993 994 995 1019 1020 1021 1022 1023

The above example will channelize my_wideband_capture.cfile, in this case a cfile captured at
925.2 MHz centered (ARFCN 975) and 20 Msps. As a result, 12 files will be generated for
ARFCNs 975 - 1023 at 1 Msps each.
"""

def eng_float(value):
    try:
        return eng_notation.str_to_num(value)
    except:
        raise ArgumentTypeError("invalid engineering notation value: {0}".format(value))

def gsm_band(value):
    choices = arfcn.get_bands()
    if value in choices:
        return value
    else:
        raise ArgumentTypeError("invalid GSM band: {0}. Possible choices are: {1}".format(value, choices))

class gsm_channelize(gr.top_block):
    def __init__(self, channels, decim, ch_width, ch_twidth, fc, band, samp_rate, path):
        gr.top_block.__init__(self, "gsm_channelize")

        ##################################################
        # Parameters
        ##################################################
        self.channels = channels
        self.decim = decim
        self.ch_width = ch_width
        self.ch_twidth = ch_twidth
        self.fc = fc
        self.band = band
        self.samp_rate = samp_rate
        self.blocks_fir_filters = {}
        self.blocks_file_sinks = {}

        ##################################################
        # Blocks and connections
        ##################################################
        self.blocks_file_source = blocks.file_source(gr.sizeof_gr_complex, path, False)

        c0_arfcn = arfcn.downlink2arfcn(fc, band)
        self.channels.insert(0, c0_arfcn)
        print("Extracting channels %s, given that the center frequency is at ARFCN %d (%s)" % (str(channels), c0_arfcn, eng_notation.num_to_str(fc)))

        for channel in channels:
            channel_freq = arfcn.arfcn2downlink(channel, band)
            if channel_freq is None:
                print("Warning: invalid ARFCN %d for band %s" % (channel, band))
                continue
            freq_diff = channel_freq - fc
            print("ARFCN %d is at C0 %+d KHz" % (channel, int(freq_diff / 1000.0)))

            self.blocks_fir_filters[channel] = filter.freq_xlating_fir_filter_ccc(decim, (firdes.low_pass(1, samp_rate, ch_width, ch_twidth)), freq_diff, samp_rate)
            self.connect((self.blocks_file_source, 0), (self.blocks_fir_filters[channel], 0))

            self.blocks_file_sinks[channel] = blocks.file_sink(gr.sizeof_gr_complex, "./gsm_channelizer/out_" + str(channel) + ".cfile", False)
            self.blocks_file_sinks[channel].set_unbuffered(False)
            self.connect((self.blocks_fir_filters[channel], 0), (self.blocks_file_sinks[channel], 0))


if __name__ == '__main__':
    parser = ArgumentParser(formatter_class=RawDescriptionHelpFormatter, description='Split wideband a GSM capture into seperate files per ARFCN.', epilog=EXTRA_HELP)
    parser.add_argument(dest="channel", type=int, nargs='+',
        help="List of ARFCNs")
    parser.add_argument("-w", "--width", dest="width", type=eng_float, default=eng_notation.num_to_str(200000),
        help="Channel lowpass width [default=%(default)s]")
    parser.add_argument("-t", "--transition-width", dest="twidth", type=eng_float, default=eng_notation.num_to_str(50000),
        help="Channel lowpass transition width [default=%(default)s]")
    parser.add_argument("-s", "--samp-rate", dest="samp_rate", type=eng_float, default=eng_notation.num_to_str(2e7),
        help="Sample rate of the wideband capture file [default=%(default)s]")
    parser.add_argument("-o", "--out-samp-rate", dest="out_samp_rate", type=eng_float, default=eng_notation.num_to_str(1e6),
        help="Sample rate of the output capture files [default=%(default)s]")
    parser.add_argument("-b", "--band", dest="band", type=gsm_band, default='E-GSM',
        help="GSM band [default=%(default)s]")
    parser.add_argument("-f", "--cfile", dest="path", type=str, required=True,
        help="Path to wideband GSM capture file")
    parser.add_argument("-c", "--C0", dest="fc", type=eng_float, default=eng_notation.num_to_str(925.2e6), required=True,
        help="C0 (BCCH) channel frequency in Hz [default=%(default)s]")
    args = parser.parse_args()

    if not os.path.exists("./gsm_channelizer"):
        os.makedirs("./gsm_channelizer")

    if not os.path.exists(args.path):
        raise IOError(args.path + " does not exist")

    if args.samp_rate % args.out_samp_rate != 0:
        raise Exception("Input sample rate should be multiple of output sample rate in order to get integer decimation.")

    decim = int(args.samp_rate / args.out_samp_rate)
    print("Input sample rate: " + eng_notation.num_to_str(args.samp_rate))
    print("Output sample rate: " + eng_notation.num_to_str(args.out_samp_rate))
    print("==> using decimation of " + str(decim))

    tb = gsm_channelize(channels=args.channel,
         decim=decim,
         ch_width=args.width,
         ch_twidth=args.twidth,
         fc=args.fc,
         band=args.band,
         samp_rate=args.samp_rate,
         path=args.path)
    tb.start()
    tb.wait()
    print("Done!")
