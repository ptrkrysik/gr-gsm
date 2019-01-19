#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
# Radio interface for Lime devices
#
# (C) 2019 by Vadim Yanitskiy <axilirator@gmail.com>
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

import limesdr

from radio_if import RadioInterface

class RadioInterfaceLMS(RadioInterface):
	LMS_DEV_TYPE_LIMESDR_MINI  = 0x01
	LMS_DEV_TYPE_LIMENET_MICRO = 0x02
	LMS_DEV_TYPE_LIMESDR_USB   = 0x03

	# FIXME: hard-coded variables shall be replaced
	# by command line arguments (or even auto-detected)!
	lms_dev_serial = "1D3937DA65C85F"
	lms_dev_type = LMS_DEV_TYPE_LIMESDR_MINI
	lms_len_tag_name = "packet_len"
	lms_dev_ch_mode = 1

	# Human-readable description
	def __str__(self):
		return "LMS"

	def phy_init_source(self):
		self._phy_src = limesdr.source(
			self.lms_dev_serial, self.lms_dev_type,
			self.lms_dev_ch_mode, 0, "")

		self._phy_src.set_sample_rate(self.sample_rate)
		self._phy_src.set_gain(self.rx_gain, 0)

	def phy_init_sink(self):
		self._phy_sink = limesdr.sink(
			self.lms_dev_serial, self.lms_dev_type,
			self.lms_dev_ch_mode, 0, "",
			self.lms_len_tag_name)

		self._phy_sink.set_sample_rate(self.sample_rate)
		self._phy_sink.set_gain(self.tx_gain, 0)

	def phy_set_rx_freq(self, freq):
		self._phy_src.set_center_freq(freq, 0)

	def phy_set_tx_freq(self, freq):
		self._phy_sink.set_center_freq(freq, 0)

	def phy_set_rx_gain(self, gain):
		self._phy_src.set_gain(gain, 0)

	def phy_set_tx_gain(self, gain):
		self._phy_sink.set_gain(gain, 0)
