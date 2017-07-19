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

from ctrl_if import CTRLInterface

class CTRLInterfaceBB(CTRLInterface):
	def __init__(self, remote_addr, remote_port, bind_port, tb):
		print("[i] Init CTRL interface")
		CTRLInterface.__init__(self, remote_addr, remote_port, bind_port)

		# Set link to the follow graph (top block)
		self.tb = tb

	def shutdown(self):
		print("[i] Shutdown CTRL interface")
		CTRLInterface.shutdown(self)

	def parse_cmd(self, request):
		# Power control
		if self.verify_cmd(request, "POWERON", 0):
			print("[i] Recv POWERON CMD")

			# Ensure transceiver isn't working
			if self.tb.trx_started:
				print("[!] Transceiver already started")
				return -1

			# Ensure transceiver is ready to start
			if not self.tb.check_available():
				print("[!] Transceiver isn't ready to start")
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
				self.tb.stop()

			return 0

		elif self.verify_cmd(request, "SETRXGAIN", 1):
			print("[i] Recv SETRXGAIN cmd")

			# TODO: check gain value
			gain = int(request[1])
			self.tb.set_gain(gain)

			return 0

		# Tuning Control
		elif self.verify_cmd(request, "RXTUNE", 1):
			print("[i] Recv RXTUNE cmd")

			# TODO: check freq range
			freq = int(request[1]) * 1000
			self.tb.set_fc(freq)

			return 0

		elif self.verify_cmd(request, "TXTUNE", 1):
			print("[i] Recv TXTUNE cmd")

			# TODO: is not implemented yet
			return 0

		# Misc
		elif self.verify_cmd(request, "ECHO", 0):
			print("[i] Recv ECHO cmd")
			return 0

		# Wrong / unknown command
		else:
			print("[!] Wrong request on CTRL interface")
			return -1
