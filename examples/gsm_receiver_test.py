#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Gsm Receiver Test
# Generated: Wed Aug  6 15:17:29 2014
##################################################

execfile("/home/piotr/.grc_gnuradio/clk_offset_corrector.py")
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from grc_gnuradio import wxgui as grc_wxgui
from optparse import OptionParser
import gsm
import wx

class gsm_receiver_test(grc_wxgui.top_block_gui):

    def __init__(self, fc=937e6, samp_rate=1e6, input_file_name="/home/piotr/Odbiornik_gsm/gr-gsm/examples/input.cfile"):
        grc_wxgui.top_block_gui.__init__(self, title="Gsm Receiver Test")
        _icon_path = "/home/piotr/.local/share/icons/hicolor/32x32/apps/gnuradio-grc.png"
        self.SetIcon(wx.Icon(_icon_path, wx.BITMAP_TYPE_ANY))

        ##################################################
        # Parameters
        ##################################################
        self.fc = fc
        self.samp_rate = samp_rate
        self.input_file_name = input_file_name

        ##################################################
        # Blocks
        ##################################################
        self.gsm_receiver_hier_0 = gsm.receiver_hier(samp_rate, 4)
        self.gsm_message_printer_0 = gsm.message_printer()
        self.gsm_get_bcch_or_ccch_bursts_0 = gsm.get_bcch_or_ccch_bursts(2)
        self.gsm_control_channels_decoder_0 = gsm.control_channels_decoder()
        self.gsm_clock_offset_control_0 = gsm.clock_offset_control(fc, samp_rate)
        self.clk_offset_corrector_0 = clk_offset_corrector(
            ppm=0,
            samp_rate=samp_rate,
            fc=936.6e6,
        )
        self.blocks_file_source_0 = blocks.file_source(gr.sizeof_gr_complex*1, input_file_name, False)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.clk_offset_corrector_0, 0), (self.gsm_receiver_hier_0, 0))
        self.connect((self.blocks_file_source_0, 0), (self.clk_offset_corrector_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.gsm_clock_offset_control_0, "ppm", self.clk_offset_corrector_0, "ppm_msg")
        self.msg_connect(self.gsm_receiver_hier_0, "measurements", self.gsm_clock_offset_control_0, "measurements")
        self.msg_connect(self.gsm_control_channels_decoder_0, "msgs", self.gsm_message_printer_0, "msgs")
        self.msg_connect(self.gsm_get_bcch_or_ccch_bursts_0, "bursts", self.gsm_control_channels_decoder_0, "bursts")
        self.msg_connect(self.gsm_receiver_hier_0, "bursts", self.gsm_get_bcch_or_ccch_bursts_0, "bursts")


    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.clk_offset_corrector_0.set_samp_rate(self.samp_rate)

    def get_input_file_name(self):
        return self.input_file_name

    def set_input_file_name(self, input_file_name):
        self.input_file_name = input_file_name
        self.blocks_file_source_0.open(self.input_file_name, False)

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
    parser.add_option("-f", "--fc", dest="fc", type="eng_float", default=eng_notation.num_to_str(937e6),
        help="Set fc [default=%default]")
    parser.add_option("-s", "--samp-rate", dest="samp_rate", type="eng_float", default=eng_notation.num_to_str(1e6),
        help="Set samp_rate [default=%default]")
    parser.add_option("-i", "--input-file-name", dest="input_file_name", type="string", default="/home/piotr/Odbiornik_gsm/gr-gsm/examples/input.cfile",
        help="Set /home/piotr/Odbiornik_gsm/gr-gsm/examples/input.cfile [default=%default]")
    (options, args) = parser.parse_args()
    tb = gsm_receiver_test(fc=options.fc, samp_rate=options.samp_rate, input_file_name=options.input_file_name)
    tb.Start(True)
    tb.Wait()
