#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Airprobe Rtlsdr
# Generated: Sat Dec  6 15:20:59 2014
##################################################

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import PyQt4.Qwt5 as Qwt
import gsm
import osmosdr
import sip
import sys

class airprobe_rtlsdr(gr.top_block, Qt.QWidget):

    def __init__(self, ppm_param=0):
        gr.top_block.__init__(self, "Airprobe Rtlsdr")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Airprobe Rtlsdr")
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

        self.settings = Qt.QSettings("GNU Radio", "airprobe_rtlsdr")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Parameters
        ##################################################
        self.ppm_param = ppm_param

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 2000000.052982
        self.ppm = ppm = ppm_param
        self.g = g = 43
        self.fc = fc = 939.4e6
        self.SDCCH = SDCCH = 6
        self.RACH = RACH = 3
        self.PCH = PCH = 5
        self.CHANNEL_UNKNOWN = CHANNEL_UNKNOWN = 0
        self.CCCH = CCCH = 2
        self.BCCH = BCCH = 1
        self.AGCH = AGCH = 4

        ##################################################
        # Blocks
        ##################################################
        self._ppm_layout = Qt.QHBoxLayout()
        self._ppm_layout.addWidget(Qt.QLabel("clock_correction [ppm]"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._ppm_counter = qwt_counter_pyslot()
        self._ppm_counter.setRange(-150, 150, 1)
        self._ppm_counter.setNumButtons(2)
        self._ppm_counter.setMinimumWidth(100)
        self._ppm_counter.setValue(self.ppm)
        self._ppm_layout.addWidget(self._ppm_counter)
        self._ppm_counter.valueChanged.connect(self.set_ppm)
        self.top_layout.addLayout(self._ppm_layout)
        self._g_layout = Qt.QHBoxLayout()
        self._g_layout.addWidget(Qt.QLabel("gain"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._g_counter = qwt_counter_pyslot()
        self._g_counter.setRange(0, 50, 0.5)
        self._g_counter.setNumButtons(2)
        self._g_counter.setMinimumWidth(100)
        self._g_counter.setValue(self.g)
        self._g_layout.addWidget(self._g_counter)
        self._g_counter.valueChanged.connect(self.set_g)
        self.top_layout.addLayout(self._g_layout)
        self._fc_layout = Qt.QVBoxLayout()
        self._fc_tool_bar = Qt.QToolBar(self)
        self._fc_layout.addWidget(self._fc_tool_bar)
        self._fc_tool_bar.addWidget(Qt.QLabel("center_frequency"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._fc_counter = qwt_counter_pyslot()
        self._fc_counter.setRange(925e6, 960e6, 2e5)
        self._fc_counter.setNumButtons(2)
        self._fc_counter.setValue(self.fc)
        self._fc_tool_bar.addWidget(self._fc_counter)
        self._fc_counter.valueChanged.connect(self.set_fc)
        self._fc_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._fc_slider.setRange(925e6, 960e6, 2e5)
        self._fc_slider.setValue(self.fc)
        self._fc_slider.setMinimumWidth(100)
        self._fc_slider.valueChanged.connect(self.set_fc)
        self._fc_layout.addWidget(self._fc_slider)
        self.top_layout.addLayout(self._fc_layout)
        self.rtlsdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + "" )
        self.rtlsdr_source_0.set_sample_rate(samp_rate)
        self.rtlsdr_source_0.set_center_freq(fc, 0)
        self.rtlsdr_source_0.set_freq_corr(ppm, 0)
        self.rtlsdr_source_0.set_dc_offset_mode(2, 0)
        self.rtlsdr_source_0.set_iq_balance_mode(2, 0)
        self.rtlsdr_source_0.set_gain_mode(True, 0)
        self.rtlsdr_source_0.set_gain(g, 0)
        self.rtlsdr_source_0.set_if_gain(20, 0)
        self.rtlsdr_source_0.set_bb_gain(20, 0)
        self.rtlsdr_source_0.set_antenna("", 0)
        self.rtlsdr_source_0.set_bandwidth(250e3, 0)
          
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	fc, #fc
        	samp_rate, #bw
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, 10)
        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.gsm_universal_ctrl_chans_demapper_0 = gsm.universal_ctrl_chans_demapper(([2,6,12,16,22,26,32,36,42,46]), ([BCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH]))
        self.gsm_receiver_0 = gsm.receiver(4, ([0]), ([]))
        self.gsm_message_printer_1 = gsm.message_printer()
        self.gsm_input_0 = gsm.gsm_input(
            ppm=0,
            osr=4,
            fc=fc,
            samp_rate_in=samp_rate,
        )
        self.gsm_control_channels_decoder_0 = gsm.control_channels_decoder()
        self.gsm_clock_offset_control_0 = gsm.clock_offset_control(fc)
        self.blocks_socket_pdu_0 = blocks.socket_pdu("UDP_CLIENT", "127.0.0.1", "4729", 10000)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.rtlsdr_source_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.rtlsdr_source_0, 0), (self.gsm_input_0, 0))
        self.connect((self.gsm_input_0, 0), (self.gsm_receiver_0, 0))

        ##################################################
        # Asynch Message Connections
        ##################################################
        self.msg_connect(self.gsm_clock_offset_control_0, "ppm", self.gsm_input_0, "ppm_in")
        self.msg_connect(self.gsm_receiver_0, "C0", self.gsm_universal_ctrl_chans_demapper_0, "bursts")
        self.msg_connect(self.gsm_control_channels_decoder_0, "msgs", self.gsm_message_printer_1, "msgs")
        self.msg_connect(self.gsm_control_channels_decoder_0, "msgs", self.blocks_socket_pdu_0, "pdus")
        self.msg_connect(self.gsm_universal_ctrl_chans_demapper_0, "bursts", self.gsm_control_channels_decoder_0, "bursts")
        self.msg_connect(self.gsm_receiver_0, "measurements", self.gsm_clock_offset_control_0, "measurements")

# QT sink close method reimplementation
    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "airprobe_rtlsdr")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_ppm_param(self):
        return self.ppm_param

    def set_ppm_param(self, ppm_param):
        self.ppm_param = ppm_param
        self.set_ppm(self.ppm_param)

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.rtlsdr_source_0.set_sample_rate(self.samp_rate)
        self.gsm_input_0.set_samp_rate_in(self.samp_rate)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.fc, self.samp_rate)

    def get_ppm(self):
        return self.ppm

    def set_ppm(self, ppm):
        self.ppm = ppm
        Qt.QMetaObject.invokeMethod(self._ppm_counter, "setValue", Qt.Q_ARG("double", self.ppm))
        self.rtlsdr_source_0.set_freq_corr(self.ppm, 0)

    def get_g(self):
        return self.g

    def set_g(self, g):
        self.g = g
        Qt.QMetaObject.invokeMethod(self._g_counter, "setValue", Qt.Q_ARG("double", self.g))
        self.rtlsdr_source_0.set_gain(self.g, 0)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        Qt.QMetaObject.invokeMethod(self._fc_counter, "setValue", Qt.Q_ARG("double", self.fc))
        Qt.QMetaObject.invokeMethod(self._fc_slider, "setValue", Qt.Q_ARG("double", self.fc))
        self.rtlsdr_source_0.set_center_freq(self.fc, 0)
        self.gsm_input_0.set_fc(self.fc)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.fc, self.samp_rate)

    def get_SDCCH(self):
        return self.SDCCH

    def set_SDCCH(self, SDCCH):
        self.SDCCH = SDCCH

    def get_RACH(self):
        return self.RACH

    def set_RACH(self, RACH):
        self.RACH = RACH

    def get_PCH(self):
        return self.PCH

    def set_PCH(self, PCH):
        self.PCH = PCH

    def get_CHANNEL_UNKNOWN(self):
        return self.CHANNEL_UNKNOWN

    def set_CHANNEL_UNKNOWN(self, CHANNEL_UNKNOWN):
        self.CHANNEL_UNKNOWN = CHANNEL_UNKNOWN

    def get_CCCH(self):
        return self.CCCH

    def set_CCCH(self, CCCH):
        self.CCCH = CCCH

    def get_BCCH(self):
        return self.BCCH

    def set_BCCH(self, BCCH):
        self.BCCH = BCCH

    def get_AGCH(self):
        return self.AGCH

    def set_AGCH(self, AGCH):
        self.AGCH = AGCH

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
    parser.add_option("-p", "--ppm-param", dest="ppm_param", type="intx", default=0,
        help="Set ppm [default=%default]")
    (options, args) = parser.parse_args()
    Qt.QApplication.setGraphicsSystem(gr.prefs().get_string('qtgui','style','raster'))
    qapp = Qt.QApplication(sys.argv)
    tb = airprobe_rtlsdr(ppm_param=options.ppm_param)
    tb.start()
    tb.show()
    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()
    tb = None #to clean up Qt widgets

