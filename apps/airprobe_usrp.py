#!/usr/bin/env python
##################################################
# Gnuradio Python Flow Graph
# Title: Airprobe Usrp
# Generated: Sat Jan  3 00:02:32 2015
##################################################

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import blocks
from gnuradio import eng_notation
from gnuradio import gr
from gnuradio import qtgui
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from optparse import OptionParser
import PyQt4.Qwt5 as Qwt
import grgsm
import pmt
import sip
import sys
import time

from distutils.version import StrictVersion
class airprobe_usrp(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Airprobe Usrp")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Airprobe Usrp")
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

        self.settings = Qt.QSettings("GNU Radio", "airprobe_usrp")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())


        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 100e6/100
        self.g = g = 30
        self.fc = fc = 1869e6
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
        self._g_layout = Qt.QHBoxLayout()
        self._g_layout.addWidget(Qt.QLabel("Gain"+": "))
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
        self._fc_tool_bar.addWidget(Qt.QLabel("Frequency"+": "))
        class qwt_counter_pyslot(Qwt.QwtCounter):
            def __init__(self, parent=None):
                Qwt.QwtCounter.__init__(self, parent)
            @pyqtSlot('double')
            def setValue(self, value):
                super(Qwt.QwtCounter, self).setValue(value)
        self._fc_counter = qwt_counter_pyslot()
        self._fc_counter.setRange(1800e6, 1900e6, 2e5)
        self._fc_counter.setNumButtons(2)
        self._fc_counter.setValue(self.fc)
        self._fc_tool_bar.addWidget(self._fc_counter)
        self._fc_counter.valueChanged.connect(self.set_fc)
        self._fc_slider = Qwt.QwtSlider(None, Qt.Qt.Horizontal, Qwt.QwtSlider.BottomScale, Qwt.QwtSlider.BgSlot)
        self._fc_slider.setRange(1800e6, 1900e6, 2e5)
        self._fc_slider.setValue(self.fc)
        self._fc_slider.setMinimumWidth(100)
        self._fc_slider.valueChanged.connect(self.set_fc)
        self._fc_layout.addWidget(self._fc_slider)
        self.top_layout.addLayout(self._fc_layout)
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("addr=192.168.10.2", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_clock_source("internal", 0)
        self.uhd_usrp_source_0.set_samp_rate(samp_rate)
        self.uhd_usrp_source_0.set_center_freq(fc, 0)
        self.uhd_usrp_source_0.set_gain(g, 0)
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
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(False)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        
        if complex == type(float()):
          self.qtgui_freq_sink_x_0.set_plot_pos_half(not True)
        
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
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])
        
        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.gsm_universal_ctrl_chans_demapper_0 = grgsm.universal_ctrl_chans_demapper(([2,6,12,16,22,26,32,36,42,46]), ([BCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH,CCCH]))
        self.gsm_receiver_0 = grgsm.receiver(4, ([0]), ([]))
        self.gsm_message_printer_0 = grgsm.message_printer(pmt.intern(""))
        self.gsm_input_0 = grgsm.gsm_input(
            ppm=0,
            osr=4,
            fc=fc,
            samp_rate_in=samp_rate,
        )
        self.gsm_control_channels_decoder_0 = grgsm.control_channels_decoder()
        self.blocks_socket_pdu_0 = blocks.socket_pdu("UDP_CLIENT", "127.0.0.1", "4729", 10000, False)
        self.blocks_message_debug_0 = blocks.message_debug()

        ##################################################
        # Connections
        ##################################################
        self.connect((self.uhd_usrp_source_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.gsm_input_0, 0))
        self.connect((self.gsm_input_0, 0), (self.gsm_receiver_0, 0))

        ##################################################
        # Message Connections
        ##################################################
        self.msg_connect(self.gsm_control_channels_decoder_0, "msgs", self.gsm_message_printer_0, "msgs")
        self.msg_connect(self.gsm_control_channels_decoder_0, "msgs", self.blocks_socket_pdu_0, "pdus")
        self.msg_connect(self.gsm_universal_ctrl_chans_demapper_0, "bursts", self.gsm_control_channels_decoder_0, "bursts")
        self.msg_connect(self.gsm_receiver_0, "C0", self.gsm_universal_ctrl_chans_demapper_0, "bursts")
        self.msg_connect(self.gsm_receiver_0, "measurements", self.blocks_message_debug_0, "print")

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "airprobe_usrp")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.gsm_input_0.set_samp_rate_in(self.samp_rate)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.fc, self.samp_rate)
        self.uhd_usrp_source_0.set_samp_rate(self.samp_rate)

    def get_g(self):
        return self.g

    def set_g(self, g):
        self.g = g
        Qt.QMetaObject.invokeMethod(self._g_counter, "setValue", Qt.Q_ARG("double", self.g))
        self.uhd_usrp_source_0.set_gain(self.g, 0)

    def get_fc(self):
        return self.fc

    def set_fc(self, fc):
        self.fc = fc
        self.gsm_input_0.set_fc(self.fc)
        self.qtgui_freq_sink_x_0.set_frequency_range(self.fc, self.samp_rate)
        self.uhd_usrp_source_0.set_center_freq(self.fc, 0)
        Qt.QMetaObject.invokeMethod(self._fc_counter, "setValue", Qt.Q_ARG("double", self.fc))
        Qt.QMetaObject.invokeMethod(self._fc_slider, "setValue", Qt.Q_ARG("double", self.fc))

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
    (options, args) = parser.parse_args()
    if(StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0")):
        Qt.QApplication.setGraphicsSystem(gr.prefs().get_string('qtgui','style','raster'))
    qapp = Qt.QApplication(sys.argv)
    tb = airprobe_usrp()
    tb.start()
    tb.show()
    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()
    tb = None #to clean up Qt widgets
