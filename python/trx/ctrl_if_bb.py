#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
# CTRL interface for OsmocomBB
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

import grgsm
from ctrl_if import ctrl_if

class ctrl_if_bb(ctrl_if):
	def __init__(self, remote_addr, remote_port, bind_addr, bind_port, tb, pm):
		ctrl_if.__init__(self, remote_addr, remote_port,
			bind_addr, bind_port)

		print("[i] Init CTRL interface (%s)" % self.desc_link())

		# Set link to the follow graph (top block)
		self.tb = tb
		# Power measurement
		self.pm = pm

	def parse_cmd(self, request):
		# Power control
		if self.verify_cmd(request, "POWERON", 0):
			print("[i] Recv POWERON CMD")

			# Ensure transceiver isn't working
			if self.tb.trx_started:
				print("[!] Transceiver already started")
				return -1

			print("[i] Starting transceiver...")
			self.tb.trx_started = True
			self.tb.start()

			return 0

		elif self.verify_cmd(request, "POWEROFF", 0):
			print("[i] Recv POWEROFF cmd")

			# TODO: flush all buffers between blocks
			if self.tb.trx_started:
				print("[i] Stopping transceiver...")
				self.tb.trx_started = False
				self.tb.set_ta(0)
				self.tb.stop()
				self.tb.wait()

			return 0

		# Gain control
		elif self.verify_cmd(request, "SETRXGAIN", 1):
			print("[i] Recv SETRXGAIN cmd")

			# TODO: check gain value
			gain = int(request[1])
			self.tb.set_rx_gain(gain)

			return 0

		elif self.verify_cmd(request, "SETTXGAIN", 1):
			print("[i] Recv SETTXGAIN cmd")

			# TODO: check gain value
			gain = int(request[1])
			self.tb.set_tx_gain(gain)

			return 0

		# Tuning Control
		elif self.verify_cmd(request, "RXTUNE", 1):
			print("[i] Recv RXTUNE cmd")

			# TODO: check freq range
			freq = int(request[1]) * 1000
			self.tb.set_rx_freq(freq)

			return 0

		elif self.verify_cmd(request, "TXTUNE", 1):
			print("[i] Recv TXTUNE cmd")

			# TODO: check freq range
			freq = int(request[1]) * 1000
			self.tb.set_tx_freq(freq)

			return 0

		# Timeslot management
		elif self.verify_cmd(request, "SETSLOT", 2):
			print("[i] Recv SETSLOT cmd")

			# Obtain TS index
			tn = int(request[1])
			if tn not in range(0, 8):
				print("[!] TS index should be in range: 0..7")
				return -1

			# Ignore timeslot type for now
			config = int(request[2])
			print("[i] Configure timeslot filter to: %s"
				% ("drop all" if config == 0 else "TS %d" % tn))

			if config == 0:
				# Value 0 means 'drop all'
				self.tb.ts_filter.set_policy(
					grgsm.FILTER_POLICY_DROP_ALL)
			else:
				self.tb.ts_filter.set_policy(
					grgsm.FILTER_POLICY_DEFAULT)
				self.tb.ts_filter.set_tn(tn)

			return 0

		# Power measurement
		elif self.verify_cmd(request, "MEASURE", 1):
			print("[i] Recv MEASURE cmd")

			# TODO: check freq range
			meas_freq = int(request[1]) * 1000

			# HACK: send fake low power values
			# until actual power measurement is implemented
			meas_dbm = str(self.pm.measure(meas_freq))

			return (0, [meas_dbm])

		# Timing Advance control
		elif self.verify_cmd(request, "SETTA", 1):
			print("[i] Recv SETTA cmd")

			# Check TA range
			ta = int(request[1])
			if ta < 0 or ta > 63:
				print("[!] TA value must be in range: 0..63")
				return -1

			self.tb.set_ta(ta)
			return 0

		# Misc
		elif self.verify_cmd(request, "ECHO", 0):
			print("[i] Recv ECHO cmd")
			return 0

		# Wrong / unknown command
		else:
			print("[!] Wrong request on CTRL interface")
			return -1
