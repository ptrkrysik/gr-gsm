#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Gsm Receiver Usrp Channelizer Python
# Generated: Sun Feb  9 12:46:36 2014
##################################################

from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.filter import pfb
from optparse import OptionParser
import gsm
import time
from numpy import fft


class gsm_receiver_usrp_channelizer_python(gr.top_block):

    def __init__(self, length=0.2, gain=25, channelizer_osr=1, fc=937e6, usrp_decim=4, fs=100e6):
        gr.top_block.__init__(self, "Gsm Receiver Usrp Channelizer Python")

        ##################################################
        # Parameters
        ##################################################
        self.length = length
        self.gain = gain
        self.channelizer_osr = channelizer_osr
        self.fc = fc
        self.usrp_decim = usrp_decim
        self.channels_num = int(round(fs/usrp_decim/200e3))
        self.fs = fs

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = 100e6/self.usrp_decim
        self.samp_rate_channelizer_out = self.samp_rate/self.channels_num*channelizer_osr
        self.freqs = fft.fftfreq(self.channels_num,1/self.samp_rate)+self.fc
        self.arfcns = get_arfcns_from_freqs(self.freqs,"e")
        print self.freqs
        print self.arfcns

        ##################################################
        # Blocks
        ##################################################
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	device_addr="addr=192.168.11.2",
        	stream_args=uhd.stream_args(
        		cpu_format="fc32",
        		otw_format="sc16",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)
        self.uhd_usrp_source_0.set_center_freq(fc, 0)
        self.uhd_usrp_source_0.set_gain(gain, 0)
        self.pfb_channelizer_ccf_0 = pfb.channelizer_ccf(
        	  self.channels_num,
        	  (),
        	  channelizer_osr,
        	  100)
        self.pfb_channelizer_ccf_0.set_channel_map(([]))

        self.blocks_head_0 = blocks.head(gr.sizeof_gr_complex*1, int(self.samp_rate*length))
        self.gsm_bursts_printer_0 = gsm.bursts_printer()
        
        self.connect((self.uhd_usrp_source_0, 0), (self.blocks_head_0, 0))
        self.connect((self.blocks_head_0, 0), (self.pfb_channelizer_ccf_0, 0))

        self.gsm_receiver_hier = {}        

        for ii in xrange(0,self.channels_num):
            self.gsm_receiver_hier[ii] = gsm.receiver_hier(self.samp_rate_channelizer_out, 4, int(self.freqs[ii]/1e5))
            self.connect((self.pfb_channelizer_ccf_0, ii), (self.gsm_receiver_hier[ii], 0))
            self.msg_connect(self.gsm_receiver_hier[ii], "bursts", self.gsm_bursts_printer_0, "bursts")
        

# QT sink close method reimplementation

    def get_length(self):
        return self.length

    def set_length(self, length):
        self.length = length

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain
        self.uhd_usrp_source_0.set_gain(self.gain, 0)

    def get_channelizer_osr(self):
        return self.channelizer_osr

    def set_channelizer_osr(self, channelizer_osr):
        self.channelizer_osr = channelizer_osr
        self.set_samp_rate_channelizer_out(self.samp_rate/self.channels_num*self.channelizer_osr)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.uhd_usrp_source_0.set_center_freq(self.fc, 0)

    def get_usrp_decim(self):
        return self.usrp_decim

    def set_usrp_decim(self, usrp_decim):
        self.usrp_decim = usrp_decim
        self.set_samp_rate(100e6/self.usrp_decim)

    def get_channels_num(self):
        return self.channels_num

    def set_channels_num(self, channels_num):
        self.channels_num = channels_num
        self.set_samp_rate_channelizer_out(self.samp_rate/self.channels_num*self.channelizer_osr)

    def get_fs(self):
        return self.fs

    def set_fs(self, fs):
        self.fs = fs

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_samp_rate_channelizer_out(self.samp_rate/self.channels_num*self.channelizer_osr)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)

    def get_samp_rate_channelizer_out(self):
        return self.samp_rate_channelizer_out

    def set_samp_rate_channelizer_out(self, samp_rate_channelizer_out):
        self.samp_rate_channelizer_out = samp_rate_channelizer_out

def get_arfcns_from_freqs(freqs,region):
    arfcns = {}
    for ii in xrange(0,len(freqs)):
        arfcns[ii] = get_arfcn_from_freq(freqs[ii],region)
    
    return arfcns

def get_arfcn_from_freq(freq,region):
	freq = freq / 1e6
	# GSM 450
	if freq <= 450.6 + 0.2*(293 - 259) + 10:
		arfcn = ((freq - (450.6 + 10)) / 0.2) + 259
	# GSM 480
	elif freq <= 479 + 0.2*(340 - 306) + 10:
		arfcn = ((freq - (479 + 10)) / 0.2) + 306
	# GSM 850
	elif freq <= 824.2 + 0.2*(251 - 128) + 45:
		arfcn = ((freq - (824.2 + 45)) / 0.2) + 128
	#E/R-GSM 900
	elif freq <= 890 + 0.2*(1023 - 1024) + 45:
		arfcn = ((freq - (890 + 45)) / -0.2) + 955
	# GSM 900
	elif freq <= 890 + 0.2*124 + 45:
		arfcn = (freq - (890 + 45)) / 0.2
	else:
		if region is "u":
			if freq > 1850.2 + 0.2*(810 - 512) + 80:
				arfcn = 0;
			else:
				arfcn = (freq - (1850.2 + 80) / 0.2) + 512
		elif region is "e":
			if freq > 1710.2 + 0.2*(885 - 512) + 95:
				arfcn = 0;
			else:
				arfcn = (freq - (1710.2 + 95) / 0.2) + 512
		else:
			arfcn = 0

	if arfcn<0:
		return 255
	else:
		return round(arfcn)
	

if __name__ == '__main__':
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    parser.add_option("", "--length", dest="length", type="eng_float", default=eng_notation.num_to_str(0.2),
        help="Set length [default=%default]")
    parser.add_option("", "--gain", dest="gain", type="intx", default=30,
        help="Set gain [default=%default]")
    parser.add_option("", "--channelizer-osr", dest="channelizer_osr", type="intx", default=2,
        help="Set channelizer_osr [default=%default]")
    parser.add_option("", "--fc", dest="fc", type="eng_float", default=eng_notation.num_to_str(937e6),
        help="Set fc [default=%default]")
    parser.add_option("", "--usrp-decim", dest="usrp_decim", type="intx", default=5,
        help="Set usrp_decim [default=%default]")
    parser.add_option("", "--fs", dest="fs", type="eng_float", default=eng_notation.num_to_str(100e6),
        help="Set fs [default=%default]")
    (options, args) = parser.parse_args()
    tb = gsm_receiver_usrp_channelizer_python(length=options.length, gain=options.gain, channelizer_osr=options.channelizer_osr, fc=options.fc, usrp_decim=options.usrp_decim, fs=options.fs)
    tb.start()
    tb.wait()


