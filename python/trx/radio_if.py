#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
# Extending a GRC flowgraph gr-gsm/examples/trx_radio_if/radio_if_grc.grc
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

from grgsm.trx import radio_if_grc

class radio_if(radio_if_grc):
	# Application state flags
	trx_started = False

	def __init__(self, phy_args, phy_sample_rate,
			phy_rx_gain, phy_tx_gain, phy_ppm,
			phy_rx_antenna, phy_tx_antenna,
			trx_remote_addr, trx_base_port,
			phy_tx_freq=938900000,
			phy_rx_freq=938900000-45e6,
			delay_correction=285.616e-6,
			uplink_shift=-(6.0/1625000*(156.25)*3),
			timing_advance=0):

		print("[i] Init Radio interface")

		radio_if_grc.__init__(self,
			samp_rate=phy_sample_rate, 
			tx_gain=phy_tx_gain, rx_gain=phy_rx_gain,
			tx_freq=phy_tx_freq, rx_freq=phy_rx_freq,
			osr=4, ppm=phy_ppm, 
			trx_base_port=str(trx_base_port),
			trx_remote_addr=trx_remote_addr,
			delay_correction=delay_correction,
			uplink_shift=uplink_shift,
			timing_advance=timing_advance)

	def shutdown(self):
		print("[i] Shutdown Radio interface")
		self.stop()
		self.wait()

