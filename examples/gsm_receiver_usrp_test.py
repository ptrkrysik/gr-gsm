#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Gsm Receiver Usrp Test
# Generated: Wed Aug  6 15:18:47 2014
##################################################

from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import uhd
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import gsm
import time
import wx

class gsm_receiver_usrp_test(grc_wxgui.top_block_gui):

    def __init__(self, length=30):
        grc_wxgui.top_block_gui.__init__(self, title="Gsm Receiver Usrp Test")
        _icon_path = "/home/piotr/.local/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Parameters
        ##################################################
        self.length = length

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 100e6/100
        self.g = g = 28
        self.fc = fc = 937e6

        ##################################################
        # Blocks
        ##################################################
        _g_sizer = wx.BoxSizer(wx.VERTICAL)
        self._g_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_g_sizer,
        	value=self.g,
        	callback=self.set_g,
        	label='g',
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._g_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_g_sizer,
        	value=self.g,
        	callback=self.set_g,
        	minimum=0,
        	maximum=50,
        	num_steps=100,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_g_sizer)
        _fc_sizer = wx.BoxSizer(wx.VERTICAL)
        self._fc_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_fc_sizer,
        	value=self.fc,
        	callback=self.set_fc,
        	label='fc',
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._fc_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_fc_sizer,
        	value=self.fc,
        	callback=self.set_fc,
        	minimum=900e6,
        	maximum=1000e6,
        	num_steps=500,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_fc_sizer)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=fc,
        	y_per_div=10,
        	y_divs=10,
        	ref_level=0,
        	ref_scale=2.0,
        	sample_rate=samp_rate,
        	fft_size=1024,
        	fft_rate=15,
        	average=False,
        	avg_alpha=None,
        	title="FFT Plot",
        	peak_hold=False,
        )
        self.GridAdd(self.wxgui_fftsink2_0.win, 1, 1, 1, 1)
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("addr=192.168.9.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_clock_source("external", 0)
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(fc, 0)
        self.uhd_usrp_source_0.set_gain(g, 0)
        self.gsm_receiver_hier_0 = gsm.receiver_hier(samp_rate, 4)
        self.gsm_get_bcch_or_ccch_bursts_0 = gsm.get_bcch_or_ccch_bursts(2)
        self.gsm_control_channels_decoder_0 = gsm.control_channels_decoder()

        ##################################################
        # Connections
        ##################################################
        self.connect((self.uhd_usrp_source_0, 0), (self.gsm_receiver_hier_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.wxgui_fftsink2_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.gsm_receiver_hier_0, "bursts", self.gsm_get_bcch_or_ccch_bursts_0, "bursts")
        self.msg_connect(self.gsm_get_bcch_or_ccch_bursts_0, "bursts", self.gsm_control_channels_decoder_0, "bursts")


    def get_length(self):
        return self.length

    def set_length(self, length):
        self.length = length

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)

    def get_g(self):
        return self.g

    def set_g(self, g):
        self.g = g
        self._g_slider.set_value(self.g)
        self._g_text_box.set_value(self.g)
        self.uhd_usrp_source_0.set_gain(self.g, 0)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.uhd_usrp_source_0.set_center_freq(self.fc, 0)
        self.wxgui_fftsink2_0.set_baseband_freq(self.fc)
        self._fc_slider.set_value(self.fc)
        self._fc_text_box.set_value(self.fc)

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"
    parser = OptionParser(option_class=eng_option, usage="%prog: [options]")
    parser.add_option("", "--length", dest="length", type="eng_float", default=eng_notation.num_to_str(30),
        help="Set length [default=%default]")
    (options, args) = parser.parse_args()
    tb = gsm_receiver_usrp_test(length=options.length)
    tb.Start(True)
    tb.Wait()
