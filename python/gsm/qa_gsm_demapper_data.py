#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2019 Vasil Velichkov <vvvelichkov@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

bursts = [format((2**(i+1))-1, '0<148b') for i in range(0,108)]
frames = range(0, 108)
timeslots = [0] * len(frames)
