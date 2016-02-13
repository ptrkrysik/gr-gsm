#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @file
# @author Roman Khassraf <rkhassraf@gmail.com>
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
from optparse import OptionParser, OptionGroup
import collections
import grgsm
import pmt
import socket


class grgsm_decoder(gr.top_block):

    def __init__(self, timeslot=0, subslot=None, chan_mode='BCCH',
                 burst_file=None,
                 cfile=None, fc=939.4e6, samp_rate=2e6, arfcn=None,
                 a5=1, a5_kc=None,
                 speech_file=None, speech_codec=None,
                 verbose=False):

        gr.top_block.__init__(self, "Gr-gsm Decode")

        ##################################################
        # Parameters
        ##################################################
        self.timeslot = timeslot
        self.subslot = subslot
        self.chan_mode = chan_mode
        self.burst_file = burst_file
        self.cfile = cfile
        self.fc = fc
        self.samp_rate = samp_rate
        self.arfcn = arfcn
        self.a5 = a5
        self.kc = a5_kc
        if len(a5_kc) < 8:
            self.kc = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
        self.speech_file = speech_file
        self.speech_codec = speech_codec
        self.verbose = verbose

        ##################################################
        # Blocks
        ##################################################

        if self.burst_file:
            self.burst_file_source = grgsm.burst_file_source(burst_file)
        elif self.cfile:
            self.file_source = blocks.file_source(gr.sizeof_gr_complex*1, self.cfile, False)
            self.receiver = grgsm.receiver(4, ([0]), ([]))
            self.input_adapter = grgsm.gsm_input(
                ppm=0,
                osr=4,
                fc=fc,
                samp_rate_in=samp_rate,
            )
            self.offset_control = grgsm.clock_offset_control(fc)

        self.dummy_burst_filter = grgsm.dummy_burst_filter()
        self.timeslot_filter = grgsm.burst_timeslot_filter(self.timeslot)

        self.subslot_filter = None
        if self.chan_mode == 'BCCH_SDCCH4' and self.subslot is not None:
            self.subslot_filter = grgsm.burst_sdcch_subslot_filter(grgsm.SS_FILTER_SDCCH4, self.subslot)
        elif self.chan_mode == 'SDCCH8' and self.subslot is not None:
            self.subslot_filter = grgsm.burst_sdcch_subslot_filter(grgsm.SS_FILTER_SDCCH8, self.subslot)

        if self.chan_mode == 'BCCH':
            self.bcch_demapper = grgsm.universal_ctrl_chans_demapper(self.timeslot,
                                                                     ([2, 6, 12, 16, 22, 26, 32, 36, 42, 46]),
                                                                     ([1, 2, 2, 2, 2, 2, 2, 2, 2, 2]))
        elif self.chan_mode == 'BCCH_SDCCH4':
            self.bcch_sdcch4_demapper = grgsm.universal_ctrl_chans_demapper(self.timeslot,
                                                                            ([2, 6, 12, 16, 22, 26, 32, 36, 42, 46]),
                                                                            ([1, 2, 2, 2, 7, 7, 7, 7, 135, 135]))
        elif self.chan_mode == 'SDCCH8':
            self.sdcch8_demapper = grgsm.universal_ctrl_chans_demapper(self.timeslot,
                                                                       ([0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44]),
                                                                       ([8, 8, 8, 8, 8, 8, 8, 8, 136, 136, 136, 136]))
        elif self.chan_mode == 'TCHF':
            self.tch_f_demapper = grgsm.tch_f_chans_demapper(self.timeslot)
            self.tch_f_decoder = grgsm.tch_f_decoder(speech_codec, speech_file)

        if self.kc != [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]:
            self.decryption = grgsm.decryption(self.kc, self.a5)
            self.cch_decoder_decrypted = grgsm.control_channels_decoder()
            if self.chan_mode == 'TCHF':
                self.decryption_tch_sacch = grgsm.decryption(self.kc, self.a5)

        self.cch_decoder = grgsm.control_channels_decoder()

        self.socket_pdu = blocks.socket_pdu("UDP_CLIENT", "127.0.0.1", "4729", 10000, False)
        if self.verbose:
            self.message_printer = grgsm.message_printer(pmt.intern(""), True, True, False)


        ##################################################
        # Asynch Message Connections
        ##################################################

        if self.burst_file:
            self.msg_connect(self.burst_file_source, "out", self.dummy_burst_filter, "in")
        elif self.cfile:
            self.connect((self.file_source, 0), (self.input_adapter, 0))
            self.connect((self.input_adapter, 0), (self.receiver, 0))
            self.msg_connect(self.offset_control, "ppm", self.input_adapter, "ppm_in")
            self.msg_connect(self.receiver, "measurements", self.offset_control, "measurements")
            self.msg_connect(self.receiver, "C0", self.dummy_burst_filter, "in")

        self.msg_connect(self.dummy_burst_filter, "out", self.timeslot_filter, "in")

        if (self.chan_mode == 'BCCH_SDCCH4' or self.chan_mode == 'SDCCH8') and self.subslot_filter is not None:
            self.msg_connect(self.timeslot_filter, "out", self.subslot_filter, "in")

        if self.chan_mode == 'BCCH':
            if self.subslot_filter is not None:
                self.msg_connect(self.subslot_filter, "out", self.bcch_demapper, "bursts")
            else:
                self.msg_connect(self.timeslot_filter, "out", self.bcch_demapper, "bursts")

            self.msg_connect(self.bcch_demapper, "bursts", self.cch_decoder, "bursts")
            self.msg_connect(self.cch_decoder, "msgs", self.socket_pdu, "pdus")
            if self.verbose:
                self.msg_connect(self.cch_decoder, "msgs", self.message_printer, "msgs")

        elif self.chan_mode == 'BCCH_SDCCH4':
            if self.subslot_filter is not None:
                self.msg_connect(self.subslot_filter, "out", self.bcch_sdcch4_demapper, "bursts")
            else:
                self.msg_connect(self.timeslot_filter, "out", self.bcch_sdcch4_demapper, "bursts")

            if self.kc != [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]:
                self.msg_connect(self.bcch_sdcch4_demapper, "bursts", self.decryption, "bursts")
                self.msg_connect(self.decryption, "bursts", self.cch_decoder_decrypted, "bursts")
                self.msg_connect(self.cch_decoder_decrypted, "msgs", self.socket_pdu, "pdus")
                if self.verbose:
                    self.msg_connect(self.cch_decoder_decrypted, "msgs", self.message_printer, "msgs")

            self.msg_connect(self.bcch_sdcch4_demapper, "bursts", self.cch_decoder, "bursts")
            self.msg_connect(self.cch_decoder, "msgs", self.socket_pdu, "pdus")
            if self.verbose:
                self.msg_connect(self.cch_decoder, "msgs", self.message_printer, "msgs")

        elif self.chan_mode == 'SDCCH8':
            if self.subslot_filter is not None:
                self.msg_connect(self.subslot_filter, "out", self.sdcch8_demapper, "bursts")
            else:
                self.msg_connect(self.timeslot_filter, "out", self.sdcch8_demapper, "bursts")

            if self.kc != [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]:
                self.msg_connect(self.sdcch8_demapper, "bursts", self.decryption, "bursts")
                self.msg_connect(self.decryption, "bursts", self.cch_decoder_decrypted, "bursts")
                self.msg_connect(self.cch_decoder_decrypted, "msgs", self.socket_pdu, "pdus")
                if self.verbose:
                    self.msg_connect(self.cch_decoder_decrypted, "msgs", self.message_printer, "msgs")

            self.msg_connect(self.sdcch8_demapper, "bursts", self.cch_decoder, "bursts")
            self.msg_connect(self.cch_decoder, "msgs", self.socket_pdu, "pdus")
            if self.verbose:
                self.msg_connect(self.cch_decoder, "msgs", self.message_printer, "msgs")

        elif self.chan_mode == 'TCHF':
            self.msg_connect(self.timeslot_filter, "out", self.tch_f_demapper, "bursts")
            if self.kc != [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]:
                self.msg_connect(self.tch_f_demapper, "acch_bursts", self.decryption_tch_sacch, "bursts")
                self.msg_connect(self.tch_f_demapper, "tch_bursts", self.decryption, "bursts")

                self.msg_connect(self.decryption_tch_sacch, "bursts", self.cch_decoder, "bursts")
                self.msg_connect(self.decryption, "bursts", self.tch_f_decoder, "bursts")
            else:
                self.msg_connect(self.tch_f_demapper, "acch_bursts", self.cch_decoder, "bursts")
                self.msg_connect(self.tch_f_demapper, "tch_bursts", self.tch_f_decoder, "bursts")

            self.msg_connect(self.tch_f_decoder, "msgs", self.socket_pdu, "pdus")
            self.msg_connect(self.cch_decoder, "msgs", self.socket_pdu, "pdus")
            if self.verbose:
                self.msg_connect(self.tch_f_decoder, "msgs", self.message_printer, "msgs")
                self.msg_connect(self.cch_decoder, "msgs", self.message_printer, "msgs")
            

if __name__ == '__main__':

    # List of channel configurations
    channel_modes = ['BCCH', 'BCCH_SDCCH4', 'SDCCH8', 'TCHF']

    # mapping options to grgsm's enums
    tch_codecs = collections.OrderedDict([
        ('FR', grgsm.TCH_FS),
        ('EFR', grgsm.TCH_EFR),
        ('AMR12.2', grgsm.TCH_AFS12_2),
        ('AMR10.2', grgsm.TCH_AFS10_2),
        ('AMR7.95', grgsm.TCH_AFS7_95),
        ('AMR7.4', grgsm.TCH_AFS7_4),
        ('AMR6.7', grgsm.TCH_AFS6_7),
        ('AMR5.9', grgsm.TCH_AFS5_9),
        ('AMR5.15', grgsm.TCH_AFS5_15),
        ('AMR4.75', grgsm.TCH_AFS4_75)
    ])

    kc = []

    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")

    def kc_callback(option, opt_str, value, parser):

        """ Callback function that parses Kc """

        # format 0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF
        if ',' in value:
            value_str = value.split(',')

            for s in value_str:
                val = int(s, 16)
                if val < 0 or val > 255:
                    parser.error("Invalid Kc % s\n" % s)
                kc.append(val)
            if len(kc) != 8:
                parser.error("Invalid Kc length")
        # format: 1234567890ABCDEF
        elif len(value) == 16:
            for i in range(8):
                s = value[2*i: 2*i + 2]
                val = int(s, 16)
                if val < 0 or val > 255:
                    parser.error("Invalid Kc % s\n" % s)
                kc.append(val)
        else:
            parser.error("Invalid Kc format")

    # define options
    parser.add_option("-m", "--mode", dest="chan_mode", default='BCCH',
                      type='choice', choices=channel_modes,
                      help="Channel mode. Valid options are 'BCCH' (Non-combined C0), "
                           "'BCCH_SDCCH4'(Combined C0), 'SDCCH8' (Stand-alone control channel) "
                           "and 'TCHF' (Traffic Channel, Full rate) ")
    parser.add_option("-t", "--timeslot", dest="timeslot", type="intx", default=0,
                      help="Timeslot to decode [default=%default]")
    parser.add_option("-u", "--subslot", dest="subslot", type="intx",
                      help="Subslot to decode. Use in combination with channel type BCCH_SDCCH4 and SDCCH8")
    parser.add_option("-b", "--burst-file", dest="burst_file", help="Input file (bursts)")
    parser.add_option("-c", "--cfile", dest="cfile", help="Input file (cfile)")
    parser.add_option("-v", "--verbose", action="store_true",
                      help="If set, the decoded messages (with frame number and count) are printed to stdout")

    # group cfile options together
    cfile_options = OptionGroup(
        parser, 'Cfile Options', 'Options for decoding cfile input.',
    )
    cfile_options.add_option("-f", "--fc", dest="fc", type="eng_float",
                             help="Frequency of cfile capture [default=eng_notation.num_to_str(939.4e6)]")
    cfile_options.add_option("-a", "--arfcn", dest="arfcn", type="intx",
                             help="Set ARFCN instead of frequency. "
                                  "In some cases you may have to provide the GSM band also")
    cfile_options.add_option("--band", dest="band",
                             help="Specify the GSM band for the frequency.\nAvailable bands are: "
                                  + ", ".join(grgsm.arfcn.get_bands()) + "."
                                  + "If no band is specified, it will be determined automatically, defaulting to 0.")
    cfile_options.add_option("-s", "--samp-rate", dest="samp_rate", type="eng_float",
                             default=eng_notation.num_to_str(2e6),
                             help="Sample rate of cfile capture [default=%default]")
    parser.add_option_group(cfile_options)

    # group decryption options
    decryption_options = OptionGroup(
        parser, 'Decryption Options', 'Options for setting the A5 decryption parameters.',
    )
    decryption_options.add_option("-e", "--a5", dest="a5", type="intx", default=1,
                                  help="A5 version [default=%default]. A5 versions 1 - 3 supported")
    decryption_options.add_option("-k", "--kc", action="callback", callback=kc_callback, type="string",
                                  help="A5 session key Kc. Valid formats are "
                                       "'0x12,0x34,0x56,0x78,0x90,0xAB,0xCD,0xEF' and '1234567890ABCDEF'")
    parser.add_option_group(decryption_options)

    # group TCH options
    tch_options = OptionGroup(
        parser, 'TCH Options', 'Options for setting Traffic channel decoding parameters.',
    )
    tch_options.add_option("-d", "--speech-codec", dest="speech_codec", default='FR',
                           type='choice', choices=tch_codecs.keys(),
                           help="TCH-F speech codec [default=%default]. "
                                "Valid options are " + ", ".join(tch_codecs.keys()))
    tch_options.add_option("-o", "--output-tch", dest="speech_output_file", default="/tmp/speech.au.gsm",
                           help="TCH/F speech output file [default=%default].")
    parser.add_option_group(tch_options)

    # parse
    (options, args) = parser.parse_args()

    # some verifications
    if (options.cfile is None and options.burst_file is None) or \
            (options.cfile is not None and options.burst_file is not None):
        parser.error("Please provide a cfile or a burst file (but not both) as input\n")

    if options.timeslot < 0 or options.timeslot > 7:
        parser.error("Invalid timeslot. Must be a in range 0-7\n")

    if options.subslot is not None and (options.subslot < 0 or options.subslot > 7):
        parser.error("Invalid subslot. Must be a in range 0-7\n")

    if options.a5 < 0 or options.a5 > 3:
        parser.error("Invalid A5 version\n")

    if options.cfile and (options.fc is None and options.arfcn is None) or (options.fc is not None and options.arfcn is not None):
        parser.error("You have to provide either a frequency or an ARFCN (but not both).\n")

    # handle frequency / arfcn input
    arfcn = 0
    fc = 939.4e6
    if options.arfcn:
        if options.band:
            if options.band not in grgsm.arfcn.get_bands():
                parser.error("Invalid GSM band\n")
            elif not grgsm.arfcn.is_valid_arfcn(options.arfcn, options.band):
                parser.error("ARFCN is not valid in the specified band\n")
            else:
                arfcn = options.arfcn
                fc = grgsm.arfcn.arfcn2downlink(arfcn, options.band)
        else:
            arfcn = options.arfcn
            for band in grgsm.arfcn.get_bands():
                if grgsm.arfcn.is_valid_arfcn(arfcn, band):
                    fc = grgsm.arfcn.arfcn2downlink(arfcn, band)
                    break
    elif options.fc:
        fc = options.fc
        if options.band:
            if options.band not in grgsm.arfcn.get_bands():
                parser.error("Invalid GSM band\n")
            elif not grgsm.arfcn.is_valid_downlink(options.fc, options.band):
                parser.error("Frequency is not valid in the specified band\n")
            else:
                arfcn = grgsm.arfcn.downlink2arfcn(options.fc, options.band)
        else:
            for band in grgsm.arfcn.get_bands():
                if grgsm.arfcn.is_valid_downlink(options.fc, band):
                    arfcn = grgsm.arfcn.downlink2arfcn(options.fc, band)
                    break

    # open udp port 4729 to avoid icmp messages
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("localhost", 4729))

    # instanciate decoder
    tb = grgsm_decoder(timeslot=options.timeslot, subslot=options.subslot, chan_mode=options.chan_mode,
                          burst_file=options.burst_file,
                          cfile=options.cfile, arfcn=arfcn, fc=fc, samp_rate=options.samp_rate,
                          a5=options.a5, a5_kc=kc,
                          speech_file=options.speech_output_file, speech_codec=tch_codecs.get(options.speech_codec),
                          verbose=options.verbose)

    # run
    tb.start()
    tb.wait()

    # we are done, close socket
    sock.close()
