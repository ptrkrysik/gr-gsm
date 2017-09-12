#!/usr/bin/env python2
# -*- coding: utf-8 -*-

# Virtual Um-interface (fake transceiver)
# Power measurement emulation for BB
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

from random import randint

class FakePM:
	# Freq. list for good power level
	bts_list = []

	def __init__(self, noise_min, noise_max, bts_min, bts_max):
		# Save power level ranges
		self.noise_min = noise_min
		self.noise_max = noise_max
		self.bts_min = bts_min
		self.bts_max = bts_max

	def measure(self, bts):
		if bts in self.bts_list:
			return randint(self.bts_min, self.bts_max)
		else:
			return randint(self.noise_min, self.noise_max)

	def update_bts_list(self, new_list):
		self.bts_list = new_list

	def add_bts_list(self, add_list):
		self.bts_list += add_list

	def del_bts_list(self, del_list):
		for item in del_list:
			if item in self.bts_list:
				self.bts_list.remove(item)
