#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later

from argparse import ArgumentParser
from math import pi

import numpy

from gnuradio import blocks
from gnuradio import gr
from gnuradio import gsm
from gnuradio.eng_arg import eng_float, intx

from wideband_receiver import wideband_receiver


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument("-s", "--samp-rate", dest="samp_rate", type=eng_float, default=2e6,
        help="Set sample rate [default=%(default)r] - allowed values even_number*0.2e6")
    parser.add_argument("-f", "--carrier-frequency", dest="carrier_frequency", type=eng_float, default=939e6,
        help="Central frequecny of GSM channel that is in the middle [default=%(default)r]")
    parser.add_argument("-i", "--file-name", dest="file_name", type=str, default="tmp.cfile",
        help="Set input file name [default=%(default)r]")
    parser.add_argument("-p", "--ppm", dest="ppm", type=intx, default=0,
        help="Set frequency correction in ppm [default=%(default)r]")
    parser.add_argument("-g", "--gain", dest="gain", type=eng_float, default=20.0,
        help="Set gain [default=%(default)r]")
    parser.add_argument("--no-record", dest="no_record", action="store_true",
        help="Do not record from the RTL-SDR device, process the already existing file given with -i")
    options = parser.parse_args()

#    socket_pdu_0_0 = blocks.socket_pdu("UDP_SERVER", "127.0.0.1", "4729", 10000, False)

    #setup recorder part
    if not options.no_record:
        import osmosdr
        recorder = gr.top_block()
        rec_len = 3 #record 3 seconds
        rtlsdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + "" )
        rtlsdr_source_0.set_sample_rate(options.samp_rate)
        rtlsdr_source_0.set_center_freq(options.carrier_frequency-0.1e6, 0) #capture half of GSM channel lower than channel center (-0.1MHz) - this is needed when even number of channels is captured in order to process full captured bandwidth
        rtlsdr_source_0.set_freq_corr(options.ppm, 0)  #correction of central frequency - if the receiver has large frequency offset value of this variable should be set close to that offset in ppm
        rtlsdr_source_0.set_dc_offset_mode(2, 0)
        rtlsdr_source_0.set_iq_balance_mode(0, 0)
        rtlsdr_source_0.set_gain_mode(True, 0)
        rtlsdr_source_0.set_bandwidth(options.samp_rate, 0)

        head_0 = blocks.head(gr.sizeof_gr_complex*1, int(rec_len*options.samp_rate))
        blocks_rotator_cc_0 = blocks.rotator_cc(-2*pi*0.1e6/options.samp_rate) #shift again by -0.1MHz in order to align channel center in 0Hz
        file_sink_0 = blocks.file_sink(gr.sizeof_gr_complex*1, options.file_name)
        file_sink_0.set_unbuffered(False)
        recorder.connect((rtlsdr_source_0, 0), (head_0, 0))
        recorder.connect((head_0, 0), (blocks_rotator_cc_0, 0))
        recorder.connect((blocks_rotator_cc_0, 0), (file_sink_0, 0))

    #setup processing part
    processing = gr.top_block()
    file_source_0 = blocks.file_source(gr.sizeof_gr_complex*1, options.file_name, False) #if file name is given process data from file
    wideband_receiver_0 = wideband_receiver(OSR=4, fc=options.carrier_frequency, samp_rate=options.samp_rate)
    gsm_extract_system_info_0 = gsm.extract_system_info()
#    socket_pdu_0 = blocks.socket_pdu("UDP_CLIENT", "127.0.0.1", "4729", 10000, False) #for some reason creation of this object prevents flowgraph from finishing
#    gsm_message_printer_0 = gsm.message_printer(pmt.intern(""), False)   #might be useful for debuggin

    processing.connect((file_source_0, 0), (wideband_receiver_0,0))
    processing.msg_connect(wideband_receiver_0, 'msgs', gsm_extract_system_info_0, 'msgs')
#    processing.msg_connect(wideband_receiver_0, 'msgs', gsm_message_printer_0, 'msgs')
#    processing.msg_connect(wideband_receiver_0, 'msgs', socket_pdu_0, 'pdus')

    if not options.no_record:
        recorder.start()
        recorder.wait()
    processing.start()
    processing.wait()

#   analysis of gathered data
    channels_num = int(options.samp_rate/0.2e6)
    freq_offsets = numpy.fft.ifftshift(numpy.array(range(int(-numpy.floor(channels_num/2)),int(numpy.floor((channels_num+1)/2))))*2e5)
    detected_c0_channels = gsm_extract_system_info_0.get_chans()
    if detected_c0_channels:
        frequencies = options.carrier_frequency+freq_offsets[(numpy.array(detected_c0_channels))]
        cell_ids = numpy.array(gsm_extract_system_info_0.get_cell_id())
        lacs = numpy.array(gsm_extract_system_info_0.get_lac())
        mncs = numpy.array(gsm_extract_system_info_0.get_mnc())
        powers = numpy.array(gsm_extract_system_info_0.get_pwrs())
        result = numpy.array([frequencies, mncs, lacs, cell_ids, powers-options.gain],dtype=object)
        result=result.transpose()
        (rows,colums)=numpy.shape(result)

        #printing to stdout
        for ff in range(0,rows):
            print('freq:{0:4.1f}M, mnc:{1:2.0f}, lac:{2:5.0f}, cid:{3:5.0f}, pwr:{4:4.0f}'.format(result[ff,0]/1e6,result[ff,1],(result[ff,2]), result[ff,3], result[ff,4]))
