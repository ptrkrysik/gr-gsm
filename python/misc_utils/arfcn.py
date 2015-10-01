#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @file
# @author Roman Khassraf <rkhassraf@gmail.com>
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


# first uplink freq, first arfcn, last arfcn, downlink frequence distance
# entries are ordered by relevance
__band_conf = collections.OrderedDict([
    ('P-GSM', {'first_freq': 890.2e6, 'first_arfcn': 1, 'last_arfcn': 124, 'downlink_dist': 45e6}),
    ('DCS1800', {'first_freq': 1710.2e6, 'first_arfcn': 512, 'last_arfcn': 885, 'downlink_dist': 95e6}),
    ('PCS1900', {'first_freq': 1850.2e6, 'first_arfcn': 512, 'last_arfcn': 810, 'downlink_dist': 80e6}),
    ('E-GSM', {'first_freq': 880.2e6, 'first_arfcn': 975, 'last_arfcn': 1023, 'downlink_dist': 45e6}),           
    ('R-GSM', {'first_freq': 876.2e6, 'first_arfcn': 955, 'last_arfcn': 1023, 'downlink_dist': 45e6}),
    ('GSM450',{'first_freq': 450.6e6, 'first_arfcn': 259, 'last_arfcn': 293, 'downlink_dist': 10e6}),
    ('GSM480', {'first_freq': 479e6, 'first_arfcn': 306, 'last_arfcn': 340, 'downlink_dist': 10e6}),
    ('GSM850', {'first_freq': 824.2e6, 'first_arfcn': 128, 'last_arfcn': 251, 'downlink_dist': 45e6})
    ])

__chan_spacing = 2e5


def get_bands():
    return __band_conf.keys()


def get_first_arfcn(band):
    """
    Returns the first arfcn (i.e. the one with the lowest number) in the given band.
    """
    if band in __band_conf:
        conf = __band_conf.get(band)
        return conf['first_arfcn']


def get_last_arfcn(band):
    """
    Returns the last arfcn (i.e. the one with the highest number) in the given band
    """
    if band in __band_conf:
        conf = __band_conf.get(band)
        return conf['last_arfcn']
    
    
def is_valid_arfcn(arfcn, band):
    """
    Returns True if arfcn is valid in the given band, else False
    """
    if band in __band_conf:
        conf = __band_conf.get(band)
        first_arfcn = conf['first_arfcn']
        last_arfcn = conf['last_arfcn']
        if first_arfcn <= arfcn <= last_arfcn:
            return True
    return False


def is_valid_uplink(freq, band):
    """
    Returns True if the given frequency is a valid uplink frequency in the given band
    """
    if band in __band_conf:
        conf = __band_conf.get(band)
        first_freq = arfcn2uplink(conf['first_arfcn'], band)
        last_freq = arfcn2uplink(conf['last_arfcn'], band)
    if first_freq is None or last_freq is None:
        return False
    if first_freq <= freq <= last_freq:
        return True
    return False


def is_valid_downlink(freq, band):
    """
    Returns True if the given frequency is a valid downlink frequency in the given band
    """
    if band in __band_conf:
        conf = __band_conf.get(band)
        first_freq = arfcn2downlink(conf['first_arfcn'], band)
        last_freq = arfcn2downlink(conf['last_arfcn'], band)
    if first_freq is None or last_freq is None:
        return False
    if first_freq <= freq <= last_freq:
        return True
    return False 


def arfcn2uplink(arfcn, band):
    if band in __band_conf and is_valid_arfcn(arfcn, band):

        conf = __band_conf.get(band)
        freq_start = conf['first_freq']
        offset = conf['first_arfcn']
        # if (band == 'E-GSM' or band == 'R-GSM') and arfcn > 124:
        #     offset = 1024
        f = freq_start + (__chan_spacing * (arfcn - offset))
        return round(f, 1)
    return None


def arfcn2downlink(arfcn, band):
    if band in __band_conf and is_valid_arfcn(arfcn, band):
        conf = __band_conf.get(band)
        distance = conf['downlink_dist']
        return round(arfcn2uplink(arfcn, band) + distance, 1)
    return None


def uplink2arfcn(freq, band):
    if band in __band_conf and is_valid_uplink(freq, band):
        conf = __band_conf.get(band)
        freq_start = conf['first_freq']
        offset = conf['first_arfcn']
        return int(round(offset + ((freq - freq_start) / __chan_spacing), 0))
    return None


def downlink2arfcn(freq, band):
    if band in __band_conf and is_valid_downlink(freq, band):
        conf = __band_conf.get(band)
        distance = conf['downlink_dist']
        freq_uplink = freq - distance
        return int(round(uplink2arfcn(freq_uplink, band), 0))
    return None
