# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

GR_PYTHON_INSTALL(
    FILES
    arfcn.py
    clock_offset_corrector_tagged.py
    device.py
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/gsm
)
