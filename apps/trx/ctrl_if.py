#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
# Transceiver UDP interface
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

import socket
import select

class UDPServer:
	def __init__(self, remote_addr, remote_port, bind_port):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		self.sock.bind(('0.0.0.0', bind_port))
		self.sock.setblocking(0)

		# Save remote info
		self.remote_addr = remote_addr
		self.remote_port = remote_port

	def loop(self):
		r_event, w_event, x_event = select.select([self.sock], [], [])

		# Check for incoming data
		if self.sock in r_event:
			data, addr = self.sock.recvfrom(128)
			self.handle_rx(data)

	def shutdown(self):
		self.sock.close();

	def send(self, data):
		self.sock.sendto(data, (self.remote_addr, self.remote_port))

	def handle_rx(self, data):
		raise NotImplementedError

class CTRLInterface(UDPServer):
	def __init__(self, remote_addr, remote_port, bind_port, radio_if):
		print("[i] Init TRX CTRL interface")
		UDPServer.__init__(self, remote_addr, remote_port, bind_port)
		self.tb = radio_if

	def shutdown(self):
		print("[i] Shutdown TRX CTRL interface")
		UDPServer.shutdown(self)

	def handle_rx(self, data):
		if self.verify_req(data):
			request = self.prepare_req(data)
			self.parse_cmd(request)
		else:
			print("[!] Wrong data on CTRL interface")

	def verify_req(self, data):
		# Verify command signature
		return data.startswith("CMD")

	def prepare_req(self, data):
		# Strip signature, paddings and \0
		request = data[4:].strip().strip("\0")
		# Split into a command and arguments
		request = request.split(" ")
		# Now we have something like ["TXTUNE", "941600"]
		return request

	def verify_cmd(self, request, cmd, argc):
		# Check if requested command matches
		if request[0] != cmd:
			return False

		# And has enough arguments
		if len(request) - 1 != argc:
			return False

		# Check if all arguments are numeric
		for v in request[1:]:
			if not v.isdigit():
				return False

		return True

	def parse_cmd(self, request):
		response_code = "0"

		# Power control
		if self.verify_cmd(request, "POWERON", 0):
			print("[i] Recv POWERON cmd")
			if not self.tb.trx_started:
				if self.tb.check_available():
					print("[i] Starting transceiver...")
					self.tb.trx_started = True
					self.tb.start()
				else:
					print("[!] Transceiver isn't ready to start")
					response_code = "-1"
			else:
				print("[!] Transceiver already started!")
				response_code = "-1"
		elif self.verify_cmd(request, "POWEROFF", 0):
			print("[i] Recv POWEROFF cmd")
			print("[i] Stopping transceiver...")
			self.tb.trx_started = False
			# TODO: flush all buffers between blocks
			self.tb.stop()
		elif self.verify_cmd(request, "SETRXGAIN", 1):
			print("[i] Recv SETRXGAIN cmd")
			# TODO: check gain value
			gain = int(request[1])
			self.tb.set_gain(gain)

		# Tuning Control
		elif self.verify_cmd(request, "RXTUNE", 1):
			print("[i] Recv RXTUNE cmd")
			# TODO: check freq range
			freq = int(request[1]) * 1000
			self.tb.set_fc(freq)
		elif self.verify_cmd(request, "TXTUNE", 1):
			print("[i] Recv TXTUNE cmd")
			# TODO: is not implemented yet

		# Misc
		elif self.verify_cmd(request, "ECHO", 0):
			print("[i] Recv ECHO cmd")

		# Wrong / unknown command
		else:
			print("[!] Wrong request on CTRL interface")
			response_code = "-1"

		# Anyway, we need to respond
		self.send_response(request, response_code)

	def send_response(self, request, response_code):
		# Include status code, for example ["TXTUNE", "0", "941600"]
		request.insert(1, response_code)
		# Add the response signature, and join back to string
		response = "RSP " + " ".join(request) + "\0"
		# Now we have something like "RSP TXTUNE 0 941600"
		self.send(response)
