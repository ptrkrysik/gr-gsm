#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2019 Vasil Velichkov <vvvelichkov@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import osmosdr
import os

def get_devices(hint=""):
    return osmosdr.device.find(osmosdr.device_t(hint))

def match(dev, filters):
    for f in filters:
        for k, v in f.items():
            if (k not in dev.to_string() or v not in dev.to_string()):
                break
        else:
            return True
    return False

def exclude(devices, filters = ({'driver': 'audio'},)):
    return [dev for dev in devices if not match(dev, filters)]

def get_all_args(hint="nofake"):
    return list(map(lambda dev: dev.to_string(), exclude(get_devices(hint))))

def get_default_args(args):
    # The presence of GRC_BLOCKS_PATH environment variable indicates that
    # gnuradio-companion compiles a flowgraph and in this case no exception
    # have to be thrown otherwise the generaged python script will be invalid.
    # This allows compilation of flowgraphs without an SDR device.
    if args or os.getenv("GRC_BLOCKS_PATH"):
        return args

    devices = get_all_args("nofake")
    print("devices: ", devices)
    if not devices:
        raise RuntimeError("Unable to find any supported SDR devices")

    return devices[0]

def print_devices(hint=""):
    devices = exclude(get_devices(hint))
    if devices:
        print("\n".join(map(lambda dev: dev.to_string(), devices)))
    else:
        print("Unable to find any supported SDR devices")
