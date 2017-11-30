#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
# Follow graph implementation
#
# (C) 2016-2017 by Vadim Yanitskiy <axilirator@gmail.com>
#
# All Rights Reserved
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

import pmt
import time
import grgsm
import osmosdr

from math import pi

from gnuradio.filter import firdes
from gnuradio import digital
from gnuradio import blocks
from gnuradio import uhd
from gnuradio import gr


# HACK: should be implemented in C++!
class burst_to_fn_time(gr.basic_block):
    def __init__(self):  # only default arguments here
        gr.basic_block.__init__(
            self,
            name='Burst to fn_time',
            in_sig=[],
            out_sig=[]
        )
        self.message_port_register_in(pmt.intern("bursts_in"))
        self.message_port_register_out(pmt.intern("fn_time_out"))
        self.set_msg_handler(pmt.intern("bursts_in"), self.convert)

    def convert(self, msg):
        fn_time = pmt.dict_ref(pmt.car(msg),pmt.intern("fn_time"),pmt.PMT_NIL)
        fn_time_msg = pmt.dict_add(pmt.make_dict(), pmt.intern("fn_time"), fn_time)
        if pmt.to_python(fn_time) is not None:
            self.message_port_pub(pmt.intern("fn_time_out"), fn_time_msg)

class radio_if(gr.top_block):
	# PHY specific variables
	rx_freq = 935e6
	tx_freq = 890e6

	# Application state flags
	trx_started = False

	# GSM timings
	delay_correction = 285.616e-6
	ul_dl_shift = -(6.0/1625000*(156.25)*3)

	def __init__(self, phy_args, phy_sample_rate,
			phy_rx_gain, phy_tx_gain, phy_ppm,
			phy_rx_antenna, phy_tx_antenna,
			trx_remote_addr, trx_base_port):

		print("[i] Init Radio interface")

		# PHY specific variables
		self.rx_gain = phy_rx_gain
		self.tx_gain = phy_tx_gain

		gr.top_block.__init__(self, "GR-GSM TRX")

		# TRX Burst Interface
		self.trx_burst_if = grgsm.trx_burst_if(
			trx_remote_addr, str(trx_base_port))


		# RX path definition
		self.phy_src = uhd.usrp_source(phy_args,
			uhd.stream_args(cpu_format="fc32",
				channels=range(1)))

		self.phy_src.set_center_freq(self.rx_freq, 0)
		self.phy_src.set_antenna(phy_rx_antenna, 0)
		self.phy_src.set_samp_rate(phy_sample_rate)
		self.phy_src.set_bandwidth(650e3, 0)
		self.phy_src.set_gain(phy_rx_gain)

		self.gsm_input = grgsm.gsm_input(
			ppm = phy_ppm - int(phy_ppm), osr = 4,
			fc = self.rx_freq, samp_rate_in = phy_sample_rate)

		self.gsm_receiver = grgsm.receiver(4, ([0]), ([]))

		self.gsm_clck_ctrl = grgsm.clock_offset_control(
			self.rx_freq, phy_sample_rate, osr = 4)

		self.ts_filter = grgsm.burst_timeslot_filter(0)
		self.ts_filter.set_policy(grgsm.FILTER_POLICY_DROP_ALL)

		# Connections
		self.connect(
			(self.phy_src, 0),
			(self.gsm_input, 0))

		self.connect(
			(self.gsm_input, 0),
			(self.gsm_receiver, 0))

		self.msg_connect(
			(self.gsm_receiver, 'C0'),
			(self.ts_filter, 'in'))

		self.msg_connect(
			(self.ts_filter, 'out'),
			(self.trx_burst_if, 'bursts'))

		self.msg_connect(
			(self.gsm_receiver, 'measurements'),
			(self.gsm_clck_ctrl, 'measurements'))

		self.msg_connect(
			(self.gsm_clck_ctrl, 'ctrl'),
			(self.gsm_input, 'ctrl_in'))


		# TX Path Definition
		self.phy_sink = uhd.usrp_sink(phy_args,
			uhd.stream_args(cpu_format="fc32",
				channels=range(1)), "packet_len")

		self.phy_sink.set_antenna(phy_tx_antenna, 0)
		self.phy_sink.set_samp_rate(phy_sample_rate)
		self.phy_sink.set_center_freq(self.tx_freq, 0)
		self.phy_sink.set_gain(self.tx_gain)

		self.tx_time_setter = grgsm.txtime_setter(
			0xffffffff, 0, 0, 0, 0, 0,
			self.delay_correction + self.ul_dl_shift)

		self.tx_burst_proc = grgsm.preprocess_tx_burst()

		self.pdu_to_tagged_stream = blocks.pdu_to_tagged_stream(
			blocks.byte_t, 'packet_len')

		self.gmsk_mod = grgsm.gsm_gmsk_mod(
			BT = 4, pulse_duration = 4, sps = 4)

		self.burst_shaper = digital.burst_shaper_cc(
			(firdes.window(firdes.WIN_HANN, 16, 0)),
			0, 20, False, "packet_len")

		# Connections
		self.msg_connect(
			(self.trx_burst_if, 'bursts'),
			(self.tx_time_setter, 'bursts_in'))

		self.msg_connect(
			(self.tx_time_setter, 'bursts_out'),
			(self.tx_burst_proc, 'bursts_in'))

		self.msg_connect(
			(self.tx_burst_proc, 'bursts_out'),
			(self.pdu_to_tagged_stream, 'pdus'))

		self.connect(
			(self.pdu_to_tagged_stream, 0),
			(self.gmsk_mod, 0))

		self.connect(
			(self.gmsk_mod, 0),
			(self.burst_shaper, 0))

		self.connect(
			(self.burst_shaper, 0),
			(self.phy_sink, 0))


		# RX & TX synchronization
		self.bt_filter = grgsm.burst_type_filter([3])
		self.burst_to_fn_time = burst_to_fn_time()

		# Connections
		self.msg_connect(
			(self.gsm_receiver, 'C0'),
			(self.bt_filter, 'bursts_in'))

		self.msg_connect(
			(self.bt_filter, 'bursts_out'),
			(self.burst_to_fn_time, 'bursts_in'))

		self.msg_connect(
			(self.burst_to_fn_time, 'fn_time_out'),
			(self.tx_time_setter, 'fn_time'))

	def shutdown(self):
		print("[i] Shutdown Radio interface")
		self.stop()
		self.wait()

	def set_rx_freq(self, fc):
		self.phy_src.set_center_freq(fc, 0)
		self.gsm_clck_ctrl.set_fc(fc)
		self.gsm_input.set_fc(fc)
		self.rx_freq = fc

	def set_tx_freq(self, fc):
		self.phy_sink.set_center_freq(fc, 0)
		self.tx_freq = fc

	def set_rx_gain(self, gain):
		self.phy_src.set_gain(gain, 0)
		self.rx_gain = gain

	def set_tx_gain(self, gain):
		self.phy_sink.set_gain(gain, 0)
		self.tx_gain = gain
