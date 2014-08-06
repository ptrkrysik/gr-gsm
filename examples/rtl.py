#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Rtl
# Generated: Wed Aug  6 15:59:37 2014
##################################################

from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import wxgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.wxgui import fftsink2
from gnuradio.wxgui import forms
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import gsm
import osmosdr
import wx

class rtl(grc_wxgui.top_block_gui):

    def __init__(self):
        grc_wxgui.top_block_gui.__init__(self, title="Rtl")
        _icon_path = "/home/piotr/.local/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 1000000.026491
        self.ppm = ppm = 0
        self.g = g = 43
        self.fc = fc = 940e6

        ##################################################
        # Blocks
        ##################################################
        _ppm_sizer = wx.BoxSizer(wx.VERTICAL)
        self._ppm_text_box = forms.text_box(
        	parent=self.GetWin(),
        	sizer=_ppm_sizer,
        	value=self.ppm,
        	callback=self.set_ppm,
        	label='ppm',
        	converter=forms.float_converter(),
        	proportion=0,
        )
        self._ppm_slider = forms.slider(
        	parent=self.GetWin(),
        	sizer=_ppm_sizer,
        	value=self.ppm,
        	callback=self.set_ppm,
        	minimum=-50,
        	maximum=50,
        	num_steps=100,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_ppm_sizer)
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
        	maximum=100,
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
        	minimum=800e6,
        	maximum=1000e6,
        	num_steps=1000,
        	style=wx.SL_HORIZONTAL,
        	cast=float,
        	proportion=1,
        )
        self.Add(_fc_sizer)
        self.wxgui_fftsink2_0 = fftsink2.fft_sink_c(
        	self.GetWin(),
        	baseband_freq=0,
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
        self.Add(self.wxgui_fftsink2_0.win)
        self.rtlsdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + "" )
        self.rtlsdr_source_0.set_sample_rate(samp_rate)
        self.rtlsdr_source_0.set_center_freq(fc, 0)
        self.rtlsdr_source_0.set_freq_corr(ppm, 0)
        self.rtlsdr_source_0.set_dc_offset_mode(0, 0)
        self.rtlsdr_source_0.set_iq_balance_mode(0, 0)
        self.rtlsdr_source_0.set_gain_mode(True, 0)
        self.rtlsdr_source_0.set_gain(g, 0)
        self.rtlsdr_source_0.set_if_gain(20, 0)
        self.rtlsdr_source_0.set_bb_gain(20, 0)
        self.rtlsdr_source_0.set_antenna("", 0)
        self.rtlsdr_source_0.set_bandwidth(250e3, 0)
          
        self.gsm_receiver_hier_0 = gsm.receiver_hier(samp_rate, 4)
        self.gsm_message_printer_0 = gsm.message_printer()
        self.gsm_get_bcch_or_ccch_bursts_0 = gsm.get_bcch_or_ccch_bursts(2)
        self.gsm_control_channels_decoder_0 = gsm.control_channels_decoder()
        self.gsm_clock_offset_control_0 = gsm.clock_offset_control(fc, samp_rate)
        self.clock_offset_corrector_0 = gsm.clock_offset_corrector(
            ppm=0,
            samp_rate=1625000.0/6.0*4.0,
            fc=936.6e6,
        )

        ##################################################
        # Connections
        ##################################################
        self.connect((self.rtlsdr_source_0, 0), (self.clock_offset_corrector_0, 0))
        self.connect((self.clock_offset_corrector_0, 0), (self.wxgui_fftsink2_0, 0))
        self.connect((self.clock_offset_corrector_0, 0), (self.gsm_receiver_hier_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.gsm_receiver_hier_0, "bursts", self.gsm_get_bcch_or_ccch_bursts_0, "bursts")
        self.msg_connect(self.gsm_get_bcch_or_ccch_bursts_0, "bursts", self.gsm_control_channels_decoder_0, "bursts")
        self.msg_connect(self.gsm_control_channels_decoder_0, "msgs", self.gsm_message_printer_0, "msgs")
        self.msg_connect(self.gsm_receiver_hier_0, "measurements", self.gsm_clock_offset_control_0, "measurements")
        self.msg_connect(self.gsm_clock_offset_control_0, "ppm", self.clock_offset_corrector_0, "ppm_msg")


    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.rtlsdr_source_0.set_sample_rate(self.samp_rate)
        self.wxgui_fftsink2_0.set_sample_rate(self.samp_rate)

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self._ppm_slider.set_value(self.ppm)
        self._ppm_text_box.set_value(self.ppm)
        self.rtlsdr_source_0.set_freq_corr(self.ppm, 0)

    def get_g(self):
        return self.g

    def set_g(self, g):
        self.g = g
        self._g_slider.set_value(self.g)
        self._g_text_box.set_value(self.g)
        self.rtlsdr_source_0.set_gain(self.g, 0)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self._fc_slider.set_value(self.fc)
        self._fc_text_box.set_value(self.fc)
        self.rtlsdr_source_0.set_center_freq(self.fc, 0)

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
    (options, args) = parser.parse_args()
    tb = rtl()
    tb.Start(True)
    tb.Wait()
