#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @file
# @author (C) 2015 by Roman Khassraf <rkhassraf@gmail.com>
# @section LICENSE
#
# Gr-gsm is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# Gr-gsm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gr-gsm; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
#
#

import collections

# first uplink freq, distance between uplink/downlink frequency, list of range tuple
# each tuple in a range tuple contains: first arfcn of the range, last arfcn of the range, offset of the range
# entries are ordered by relevance
__band_conf = collections.OrderedDict([
    ('P-GSM', {'f_start': 890.0e6, 'distance': 45e6, 'ranges': [(1, 124, 0)]}),
    ('DCS1800', {'f_start': 1710.2e6, 'distance': 95e6, 'ranges': [(512, 885, 512)]}),
    ('PCS1900', {'f_start': 1850.2e6, 'distance': 80e6, 'ranges': [(512, 810, 512)]}),
    ('E-GSM', {'f_start': 890.0e6, 'distance': 45e6, 'ranges': [(0, 124, 0), (975, 1023, 1024)]}),
    ('R-GSM', {'f_start': 890.0e6, 'distance': 45e6, 'ranges': [(0, 124, 0), (955, 1023, 1024)]}),
    ('GSM450', {'f_start': 450.6e6, 'distance': 10e6, 'ranges': [(259, 293, 259)]}),
    ('GSM480', {'f_start': 479e6, 'distance': 10e6, 'ranges': [(306, 340, 306)]}),
    ('GSM850', {'f_start': 824.2e6, 'distance': 45e6, 'ranges': [(128, 251, 128)]}),
])

__chan_spacing = 2e5


def get_bands():
    return __band_conf.keys()


def is_valid_arfcn(arfcn, band):
    """
    Returns True if arfcn is valid in the given band, else False
    """
    if band in __band_conf:
        conf = __band_conf.get(band)
        for arfcn_range in conf['ranges']:
            arfcn_start = arfcn_range[0]
            arfcn_end = arfcn_range[1]
            if arfcn_start <= arfcn <= arfcn_end:
                return True
    return False


def is_valid_uplink(freq, band):
    """
    Returns True if the given frequency is a valid uplink frequency in the given band
    """
    result = None
    if band in __band_conf:
        conf = __band_conf.get(band)
        result = False
        for arfcn_range in conf['ranges']:
            arfcn_start = arfcn_range[0]
            arfcn_end = arfcn_range[1]
            first_freq = arfcn2uplink(arfcn_start, band)
            last_freq = arfcn2uplink(arfcn_end, band)
            if first_freq is None or last_freq is None:
                result = False
            elif first_freq <= freq <= last_freq:
                result = True
    return result


def is_valid_downlink(freq, band):
    """
    Returns True if the given frequency is a valid downlink frequency in the given band
    """
    result = None
    if band in __band_conf:
        conf = __band_conf.get(band)
        result = False
        for arfcn_range in conf['ranges']:
            arfcn_start = arfcn_range[0]
            arfcn_end = arfcn_range[1]
            first_freq = arfcn2downlink(arfcn_start, band)
            last_freq = arfcn2downlink(arfcn_end, band)
            if first_freq is None or last_freq is None:
                result = False
            elif first_freq <= freq <= last_freq:
                result = True
    return result


def arfcn2uplink(arfcn, band):
    if band in __band_conf and is_valid_arfcn(arfcn, band):
        conf = __band_conf.get(band)
        f_start = conf['f_start']
        offset = None
        for arfcn_range in conf['ranges']:
            arfcn_start = arfcn_range[0]
            arfcn_end = arfcn_range[1]
            if arfcn_start <= arfcn <= arfcn_end:
                offset = arfcn_range[2]

        if offset is not None:
            f = f_start + (__chan_spacing * (arfcn - offset))
            return round(f, 1)
    return -1


def arfcn2downlink(arfcn, band):
    if band in __band_conf and is_valid_arfcn(arfcn, band):
        conf = __band_conf.get(band)
        distance = conf['distance']
        return round(arfcn2uplink(arfcn, band) + distance, 1)
    return -1


def uplink2arfcn(freq, band):
    if band in __band_conf and is_valid_uplink(freq, band):
        conf = __band_conf.get(band)
        f_start = conf['f_start']
        offset = None
        for arfcn_range in conf['ranges']:
            arfcn_start = arfcn_range[0]
            arfcn_end = arfcn_range[1]
            offset = arfcn_range[2]
            arfcn = int(round(offset + ((freq - f_start) / __chan_spacing), 0))
            if arfcn_start <= arfcn <= arfcn_end:
                return arfcn
    return -1


def downlink2arfcn(freq, band):
    if band in __band_conf and is_valid_downlink(freq, band):
        conf = __band_conf.get(band)
        distance = conf['distance']
        freq_uplink = freq - distance
        return int(round(uplink2arfcn(freq_uplink, band), 0))
    return -1


def get_arfcn_ranges(band):
    """
    Returns a list of arfcn tuples, each with first and last arfcn of the range.
    """
    result = []
    if band in __band_conf:
        conf = __band_conf.get(band)
        for arfcn_range in conf['ranges']:
            arfcn_tuple = (arfcn_range[0], arfcn_range[1])
            result.append(arfcn_tuple)
    return result
