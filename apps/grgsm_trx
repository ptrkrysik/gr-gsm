#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# GR-GSM based transceiver
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

import signal
import getopt
import sys

from ctrl_if_bb import CTRLInterfaceBB
from radio_if import RadioInterface
from fake_pm import FakePM

COPYRIGHT = \
	"Copyright (C) 2016-2017 by Vadim Yanitskiy <axilirator@gmail.com>\n" \
	"License GPLv2+: GNU GPL version 2 or later " \
	"<http://gnu.org/licenses/gpl.html>\n" \
	"This is free software: you are free to change and redistribute it.\n" \
	"There is NO WARRANTY, to the extent permitted by law.\n"

class Application:
	# Application variables
	remote_addr = "127.0.0.1"
	base_port = 5700

	# PHY specific
	phy_sample_rate = 2000000
	phy_subdev_spec = False
	phy_gain = 30
	phy_args = ""
	phy_ppm = 0

	def __init__(self):
		self.print_copyright()
		self.parse_argv()

		# Set up signal handlers
		signal.signal(signal.SIGINT, self.sig_handler)

	def run(self):
		# Init Radio interface
		self.radio = RadioInterface(self.phy_args, self.phy_subdev_spec,
			self.phy_sample_rate, self.phy_gain, self.phy_ppm,
			self.remote_addr, self.base_port)

		# Power measurement emulation
		# Noise: -120 .. -105
		# BTS: -75 .. -50
		self.pm = FakePM(-120, -105, -75, -50)

		# Init TRX CTRL interface
		self.server = CTRLInterfaceBB(self.remote_addr,
			self.base_port + 101, self.base_port + 1,
			self.radio, self.pm)

		print("[i] Init complete")

		# Enter main loop
		while True:
			self.server.loop()

	def shutdown(self):
		print("[i] Shutting down...")
		self.server.shutdown()
		self.radio.shutdown()

	def print_copyright(self):
		print(COPYRIGHT)

	def print_help(self):
		s  = " Usage: " + sys.argv[0] + " [options]\n\n" \
			 " Some help...\n" \
			 "  -h --help         this text\n\n"

		# TRX specific
		s += " TRX interface specific\n" \
			 "  -s --remote-addr  Set remote address (default 127.0.0.1)\n" \
			 "  -p --base-port    Set base port number (default 5700)\n\n"

		# PHY specific
		s += " Radio interface specific\n" \
			 "  -a --device-args  Set device arguments\n" \
			 "  -s --sample-rate  Set PHY sample rate (default 2000000)\n" \
			 "  -S --subdev-spec  Set PHY sub-device specification\n" \
			 "  -g --gain         Set PHY gain (default 30)\n" \
			 "     --ppm          Set PHY frequency correction (default 0)\n"

		print(s)

	def parse_argv(self):
		try:
			opts, args = getopt.getopt(sys.argv[1:],
				"a:p:i:s:S:g:h",
				["help", "remote-addr=", "base-port=", "device-args=",
				"gain=", "subdev-spec=", "sample-rate=", "ppm="])
		except getopt.GetoptError as err:
			# Print(help and exit)
			self.print_help()
			print("[!] " + str(err))
			sys.exit(2)

		for o, v in opts:
			if o in ("-h", "--help"):
				self.print_help()
				sys.exit(2)

			# TRX specific
			elif o in ("-i", "--remote-addr"):
				self.remote_addr = v
			elif o in ("-p", "--base-port"):
				if int(v) >= 0 and int(v) <= 65535:
					self.base_port = int(v)
				else:
					print("[!] The port number should be in range [0-65536]")
					sys.exit(2)

			# PHY specific
			elif o in ("-a", "--device-args"):
				self.phy_args = v
			elif o in ("-g", "--gain"):
				self.phy_gain = int(v)
			elif o in ("-S", "--subdev-spec"):
				self.phy_subdev_spec = v
			elif o in ("-s", "--sample-rate"):
				self.phy_sample_rate = int(v)
			elif o in ("--ppm"):
				self.phy_ppm = int(v)

	def sig_handler(self, signum, frame):
		print("Signal %d received" % signum)
		if signum is signal.SIGINT:
			self.shutdown()
			sys.exit(0)

if __name__ == '__main__':
	app = Application()
	app.run()
