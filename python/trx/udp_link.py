#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
# UDP link implementation
#
# (C) 2017 by Vadim Yanitskiy <axilirator@gmail.com>
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

class udp_link:
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
			self.handle_rx(data.decode())

	def shutdown(self):
		self.sock.close();

	def send(self, data):
		if type(data) not in [bytearray, bytes]:
			data = data.encode()

		self.sock.sendto(data, (self.remote_addr, self.remote_port))

	def handle_rx(self, data):
		raise NotImplementedError
