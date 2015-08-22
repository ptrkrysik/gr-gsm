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

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot

from distutils.version import StrictVersion
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from math import pi
from optparse import OptionParser

import PyQt4.Qwt5 as Qwt
import grgsm
import osmosdr
import pmt
import sip


class airprobe_rtlsdr_capture(gr.top_block, Qt.QWidget):

    def __init__(self, fc, gain, samp_rate, ppm, arfcn, cfile=None, burst_file=None, verbose=False):

        gr.top_block.__init__(self, "Airprobe RTL-SDR Capture")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Airprobe RTL-SDR Capture")
        try:
             self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
             pass

        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "airprobe_rtlsdr_capture")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


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

        ##################################################
        # Variables
        ##################################################
        self.ppm_slider = ppm_slider = ppm
        self.g_slider = g_slider = gain
        self.fc_slider = fc_slider = fc

        ##################################################
        # UI Blocks
        ##################################################
        self._ppm_slider_layout = Qt.QHBoxLayout()
        self._ppm_slider_layout.addWidget(Qt.QLabel("PPM Offset"+": "))

        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)

            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)

        self._ppm_slider_counter = qwt_counter_pyslot()
        self._ppm_slider_counter.setRange(-150, 150, 1)
        self._ppm_slider_counter.setNumButtons(2)
        self._ppm_slider_counter.setMinimumWidth(100)
        self._ppm_slider_counter.setValue(self.ppm_slider)
        self._ppm_slider_layout.addWidget(self._ppm_slider_counter)
        self._ppm_slider_counter.valueChanged.connect(self.set_ppm_slider)
        self.top_layout.addLayout(self._ppm_slider_layout)
        self._g_slider_layout = Qt.QHBoxLayout()
        self._g_slider_layout.addWidget(Qt.QLabel("Gain"+": "))

        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)

            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)

        self._g_slider_counter = qwt_counter_pyslot()
        self._g_slider_counter.setRange(0, 50, 0.5)
        self._g_slider_counter.setNumButtons(2)
        self._g_slider_counter.setMinimumWidth(100)
        self._g_slider_counter.setValue(self.g_slider)
        self._g_slider_layout.addWidget(self._g_slider_counter)
        self._g_slider_counter.valueChanged.connect(self.set_g_slider)

        self.top_layout.addLayout(self._g_slider_layout)

        self._fc_slider_layout = Qt.QVBoxLayout()
        self._fc_slider_tool_bar = Qt.QToolBar(self)
        self._fc_slider_layout.addWidget(self._fc_slider_tool_bar)
        self._fc_slider_tool_bar.addWidget(Qt.QLabel("Frequency"+": "))

        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)

            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)

        self._fc_slider_counter = qwt_counter_pyslot()
        self._fc_slider_counter.setRange(925e6, 1990e6, 2e5)
        self._fc_slider_counter.setNumButtons(2)
        self._fc_slider_counter.setValue(self.fc_slider)

        self._fc_slider_tool_bar.addWidget(self._fc_slider_counter)

        self._fc_slider_counter.valueChanged.connect(self.set_fc_slider)

        self._fc_slider_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._fc_slider_slider.setRange(925e6, 1990e6, 2e5)
        self._fc_slider_slider.setValue(self.fc_slider)
        self._fc_slider_slider.setMinimumWidth(100)
        self._fc_slider_slider.valueChanged.connect(self.set_fc_slider)
        self._fc_slider_layout.addWidget(self._fc_slider_slider)

        self.top_layout.addLayout(self._fc_slider_layout)

        self.qtgui_freq_sink_x = qtgui.freq_sink_c(
            1024,
            firdes.WIN_BLACKMAN_hARRIS,
            fc_slider,
            samp_rate,
            "",
            1
        )
        self.qtgui_freq_sink_x.set_update_time(0.10)
        self.qtgui_freq_sink_x.set_y_axis(-140, 10)
        self.qtgui_freq_sink_x.enable_autoscale(False)
        self.qtgui_freq_sink_x.enable_grid(False)
        self.qtgui_freq_sink_x.set_fft_average(1.0)

        labels = ["", "", "", "", "",
                  "", "", "", "", ""]
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_win = sip.wrapinstance(self.qtgui_freq_sink_x.pyqwidget(), Qt.QWidget)

        self.top_layout.addWidget(self._qtgui_freq_sink_x_win)


        ##################################################
        # Processing Blocks
        ##################################################

        self.rtlsdr_source = osmosdr.source( args="numchan=" + str(1) + " " + "" )
        self.rtlsdr_source.set_sample_rate(samp_rate)
        self.rtlsdr_source.set_center_freq(fc_slider - shiftoff, 0)
        self.rtlsdr_source.set_freq_corr(ppm_slider, 0)
        self.rtlsdr_source.set_dc_offset_mode(2, 0)
        self.rtlsdr_source.set_iq_balance_mode(2, 0)
        self.rtlsdr_source.set_gain_mode(True, 0)
        self.rtlsdr_source.set_gain(g_slider, 0)
        self.rtlsdr_source.set_if_gain(20, 0)
        self.rtlsdr_source.set_bb_gain(20, 0)
        self.rtlsdr_source.set_antenna("", 0)
        self.rtlsdr_source.set_bandwidth(250e3+abs(shiftoff), 0)
        self.blocks_rotator = blocks.rotator_cc(-2*pi*shiftoff/samp_rate)

        if self.verbose or self.burst_file:
            self.gsm_receiver = grgsm.receiver(4, ([self.arfcn]), ([]))
            self.gsm_input = grgsm.gsm_input(
                ppm=0,
                osr=4,
                fc=fc,
                samp_rate_in=samp_rate,
            )
            self.gsm_clock_offset_control = grgsm.clock_offset_control(fc-shiftoff)

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

        self.connect((self.rtlsdr_source, 0), (self.blocks_rotator, 0))
        self.connect((self.blocks_rotator, 0), (self.qtgui_freq_sink_x, 0))

        if self.cfile:
            self.connect((self.blocks_rotator, 0), (self.blocks_file_sink, 0))

        if self.verbose or self.burst_file:
            self.connect((self.gsm_input, 0), (self.gsm_receiver, 0))
            self.connect((self.blocks_rotator, 0), (self.gsm_input, 0))
            self.msg_connect(self.gsm_clock_offset_control, "ppm", self.gsm_input, "ppm_in")
            self.msg_connect(self.gsm_receiver, "measurements", self.gsm_clock_offset_control, "measurements")

            if self.burst_file:
                self.msg_connect(self.gsm_receiver, "C0", self.gsm_burst_file_sink, "in")
            if self.verbose:
                self.msg_connect(self.gsm_receiver, "C0", self.gsm_bursts_printer_0, "bursts")

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "airprobe_rtlsdr_capture")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.set_fc_slider(self.fc)
        if self.verbose or self.burst_file:
            self.gsm_input.set_fc(self.fc)

    def get_gain(self):
        return self.gain

    def set_gain(self, gain):
        self.gain = gain
        self.set_g_slider(self.gain)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_freq_sink_x.set_frequency_range(self.fc_slider, self.samp_rate)
        self.rtlsdr_source.set_sample_rate(self.samp_rate)
        if self.verbose or self.burst_file:
            self.gsm_input.set_samp_rate_in(self.samp_rate)

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        self.set_ppm_slider(self.ppm)

    def get_ppm_slider(self):
        return self.ppm_slider

    def set_ppm_slider(self, ppm_slider):
        self.ppm_slider = ppm_slider
        Qt.QMetaObject.invokeMethod(self._ppm_slider_counter, "setValue", Qt.Q_ARG("double", self.ppm_slider))
        self.rtlsdr_source.set_freq_corr(self.ppm_slider, 0)

    def get_g_slider(self):
        return self.g_slider

    def set_g_slider(self, g_slider):
        self.g_slider = g_slider
        Qt.QMetaObject.invokeMethod(self._g_slider_counter, "setValue", Qt.Q_ARG("double", self.g_slider))
        self.rtlsdr_source.set_gain(self.g_slider, 0)

    def get_fc_slider(self):
        return self.fc_slider

    def set_fc_slider(self, fc_slider):
        self.fc_slider = fc_slider
        Qt.QMetaObject.invokeMethod(self._fc_slider_counter, "setValue", Qt.Q_ARG("double", self.fc_slider))
        Qt.QMetaObject.invokeMethod(self._fc_slider_slider, "setValue", Qt.Q_ARG("double", self.fc_slider))
        self.qtgui_freq_sink_x.set_frequency_range(self.fc_slider, self.samp_rate)
        self.rtlsdr_source.set_center_freq(self.fc_slider, 0)

if __name__ == '__main__':
    import ctypes
    import sys

    parser = OptionParser(option_class=eng_option, usage="%prog [options]",
                          description="RTL-SDR capturing app of gr-gsm.")
    #parser.formatter.width = 80

    parser.add_option("-f", "--fc", dest="fc", type="eng_float", 
                      help="Set frequency [default=%default]")
    
    parser.add_option("-a", "--arfcn", dest="arfcn", type="intx", 
                      help="Set ARFCN instead of frequency. In some cases you may have to provide the GSM band also")
    
    parser.add_option("-g", "--gain", dest="gain", type="eng_float", 
                      default=eng_notation.num_to_str(30),
                      help="Set gain [default=%default]")
    
    parser.add_option("-s", "--samp-rate", dest="samp_rate", type="eng_float", 
                      default=eng_notation.num_to_str(2000000.052982),
                      help="Set samp_rate [default=%default]")
    
    parser.add_option("-p", "--ppm", dest="ppm", type="intx", default=0,
                      help="Set ppm [default=%default]")
    
    parser.add_option("-b", "--burst-file", dest="burst_file", 
                      help="File where the captured bursts are saved")
    
    parser.add_option("-c", "--cfile", dest="cfile", 
                      help="File where the captured data are saved")
    
    parser.add_option("-v", "--verbose", action="store_true", 
                      help="If set, the captured bursts are printed to stdout")
    
    bands_list = ", ".join(grgsm.arfcn.get_bands())
    parser.add_option("--band", dest="band", 
                      help="Specify the GSM band for the frequency.\nAvailable bands are: " + bands_list + ".\nIf no band is specified, it will be determined automatically, defaulting to 0." )

    (options, args) = parser.parse_args()
    
    if options.cfile is None and options.burst_file is None:
        parser.error("Please provide a cfile or a burst file (or both) to save the captured data\n")
        
    if (options.fc is None and options.arfcn is None) or (options.fc is not None and options.arfcn is not None):
        parser.error("You have to provide either a frequency or an ARFCN (but not both).\n")
        
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
     

    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        Qt.QApplication.setGraphicsSystem(gr.prefs().get_string('qtgui','style','raster'))
    qapp = Qt.QApplication(sys.argv)

    tb = airprobe_rtlsdr_capture(fc=fc, gain=options.gain, samp_rate=options.samp_rate,
                         ppm=options.ppm, arfcn=arfcn, cfile=options.cfile, burst_file=options.burst_file, verbose=options.verbose)

    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()

    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()
    tb = None #to clean up Qt widgets
