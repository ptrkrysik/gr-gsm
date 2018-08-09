#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
# Follow graph implementation
#
# (C) 2016-2017 by Vadim Yanitskiy <axilirator@gmail.com>
# (C) 2017      by Piotr Krysik <ptrkrysik@gmail.com>
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

from math import pi

from gnuradio import digital
from gnuradio import blocks
from gnuradio import uhd
from gnuradio import gr

from gnuradio import filter
from gnuradio.filter import firdes


# HACK: should be implemented in C++!
class dict_toggle_sign(gr.basic_block):
    def __init__(self):  # only default arguments here
        gr.basic_block.__init__(self,
            name='Change sign of elts in dict',
            in_sig=[],
            out_sig=[]
        )
        self.message_port_register_in(pmt.intern("dict_in"))
        self.message_port_register_out(pmt.intern("dict_out"))
        self.set_msg_handler(pmt.intern("dict_in"), self.change_sign)

    def change_sign(self, msg):
        if pmt.is_dict(msg):
            d = pmt.to_python(msg)
            for key, value in d.items():
                d[key] *= -1
            self.message_port_pub(pmt.intern("dict_out"), pmt.to_pmt(d))

class radio_if(gr.top_block):
	# PHY specific variables
	rx_freq = 935e6
	tx_freq = 890e6
	osr = 4

	# Application state flags
	trx_started = False

	# GSM timings (in microseconds [uS])
	# One timeslot duration is 576.9 μs = 15/26 ms,
	# or 156.25 symbol periods (a symbol period is 48/13 μs)
	GSM_SYM_PERIOD_uS = 48.0 / 13.0
	GSM_TS_PERIOD_uS = GSM_SYM_PERIOD_uS * 156.25
	GSM_UL_DL_SHIFT_uS = -(GSM_TS_PERIOD_uS * 3)

	# FIXME: shall be measured (automatically?) for
	# particular device and particular clock rate.
	# The current value is measured for USRP B2X0 at 26e6.
	delay_correction = (285.616 + 2 * GSM_SYM_PERIOD_uS) * 1e-6

	def __init__(self, phy_args, phy_sample_rate,
			phy_rx_gain, phy_tx_gain, phy_ppm,
			phy_rx_antenna, phy_tx_antenna,
			trx_bind_addr, trx_remote_addr,
			trx_base_port):

		print("[i] Init Radio interface (L:%s:%u <-> R:%s:%u)"
			% (trx_bind_addr, trx_base_port + 2,
				trx_remote_addr, trx_base_port + 102))

		# PHY specific variables
		self.sample_rate = phy_sample_rate
		self.rx_gain = phy_rx_gain
		self.tx_gain = phy_tx_gain
		self.ppm = phy_ppm

		gr.top_block.__init__(self, "GR-GSM TRX")

		# TRX Burst Interface
		self.trx_burst_if = grgsm.trx_burst_if(
			trx_bind_addr, trx_remote_addr,
			str(trx_base_port))

		# RX path definition
		self.phy_src = uhd.usrp_source(phy_args,
			uhd.stream_args(cpu_format="fc32",
				channels=range(1)))

		self.phy_src.set_clock_rate(26e6, uhd.ALL_MBOARDS)
		self.phy_src.set_center_freq(self.rx_freq, 0)
		self.phy_src.set_antenna(phy_rx_antenna, 0)
		self.phy_src.set_samp_rate(phy_sample_rate)
		self.phy_src.set_bandwidth(650e3, 0)
		self.phy_src.set_gain(phy_rx_gain)

		self.msg_to_tag_src = grgsm.msg_to_tag()

		self.rotator_src = grgsm.controlled_rotator_cc(
			self.calc_phase_inc(self.rx_freq))

		self.lpf = filter.fir_filter_ccf(1, firdes.low_pass(
			1, phy_sample_rate, 125e3, 5e3, firdes.WIN_HAMMING, 6.76))

		self.gsm_receiver = grgsm.receiver(self.osr, ([0]), ([]))

		self.ts_filter = grgsm.burst_timeslot_filter(0)
		self.ts_filter.set_policy(grgsm.FILTER_POLICY_DROP_ALL)

		# Connections
		self.connect(
			(self.phy_src, 0),
			(self.msg_to_tag_src, 0))

		self.connect(
			(self.msg_to_tag_src, 0),
			(self.rotator_src, 0))

		self.connect(
			(self.rotator_src, 0),
			(self.lpf, 0))

		self.connect(
			(self.lpf, 0),
			(self.gsm_receiver, 0))

		self.msg_connect(
			(self.gsm_receiver, 'C0'),
			(self.ts_filter, 'in'))

		self.msg_connect(
			(self.ts_filter, 'out'),
			(self.trx_burst_if, 'bursts'))


		# TX Path Definition
		self.phy_sink = uhd.usrp_sink(phy_args,
			uhd.stream_args(cpu_format="fc32",
				channels=range(1)), "packet_len")

		self.phy_sink.set_clock_rate(26e6, uhd.ALL_MBOARDS)
		self.phy_sink.set_antenna(phy_tx_antenna, 0)
		self.phy_sink.set_samp_rate(phy_sample_rate)
		self.phy_sink.set_center_freq(self.tx_freq, 0)
		self.phy_sink.set_gain(self.tx_gain)

		self.tx_time_setter = grgsm.txtime_setter(
			0xffffffff, 0, 0, 0, 0, 0,
			self.delay_correction + self.GSM_UL_DL_SHIFT_uS * 1e-6)

		self.tx_burst_proc = grgsm.preprocess_tx_burst()

		self.pdu_to_tagged_stream = blocks.pdu_to_tagged_stream(
			blocks.byte_t, 'packet_len')

		self.gmsk_mod = grgsm.gsm_gmsk_mod(
			BT = 0.3, pulse_duration = 4, sps = self.osr)

		self.burst_shaper = digital.burst_shaper_cc(
			(firdes.window(firdes.WIN_HANN, 16, 0)),
			0, 20, False, "packet_len")

		self.msg_to_tag_sink = grgsm.msg_to_tag()

		self.rotator_sink = grgsm.controlled_rotator_cc(
			-self.calc_phase_inc(self.tx_freq))

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
			(self.msg_to_tag_sink, 0))

		self.connect(
			(self.msg_to_tag_sink, 0),
			(self.rotator_sink, 0))

		self.connect(
			(self.rotator_sink, 0),
			(self.phy_sink, 0))


		# RX & TX synchronization
		self.bt_filter = grgsm.burst_type_filter([3])
		self.burst_to_fn_time = grgsm.burst_to_fn_time()

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


		# AFC (Automatic Frequency Correction)
		self.gsm_clck_ctrl = grgsm.clock_offset_control(
			self.rx_freq, phy_sample_rate, osr = self.osr)

		self.dict_toggle_sign = dict_toggle_sign()

		# Connections
		self.msg_connect(
			(self.gsm_receiver, 'measurements'),
			(self.gsm_clck_ctrl, 'measurements'))

		self.msg_connect(
			(self.gsm_clck_ctrl, 'ctrl'),
			(self.msg_to_tag_src, 'msg'))

		self.msg_connect(
			(self.gsm_clck_ctrl, 'ctrl'),
			(self.dict_toggle_sign, 'dict_in'))

		self.msg_connect(
			(self.dict_toggle_sign, 'dict_out'),
			(self.msg_to_tag_sink, 'msg'))


		# Some UHD devices (such as UmTRX) do start the clock
		# not from 0, so it's required to reset it manually.
		# Resetting UHD source will also affect the sink.
		self.phy_src.set_time_now(uhd.time_spec(0.0))

	def shutdown(self):
		print("[i] Shutdown Radio interface")
		self.stop()
		self.wait()

	def calc_phase_inc(self, fc):
		return self.ppm / 1.0e6 * 2 * pi * fc / self.sample_rate

	def set_rx_freq(self, fc):
		self.phy_src.set_center_freq(fc, 0)
		self.rotator_src.set_phase_inc(self.calc_phase_inc(fc))
		self.rx_freq = fc

	def set_tx_freq(self, fc):
		self.phy_sink.set_center_freq(fc, 0)
		self.rotator_sink.set_phase_inc(-self.calc_phase_inc(fc))
		self.tx_freq = fc

	def set_rx_gain(self, gain):
		self.phy_src.set_gain(gain, 0)
		self.rx_gain = gain

	def set_tx_gain(self, gain):
		self.phy_sink.set_gain(gain, 0)
		self.tx_gain = gain

	def set_ta(self, ta):
		print("[i] Setting TA value %d" % ta)
		advance_time_sec = ta * self.GSM_SYM_PERIOD_uS * 1e-6
		self.tx_time_setter.set_timing_advance(advance_time_sec)
