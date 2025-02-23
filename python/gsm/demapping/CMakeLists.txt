# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

GR_PYTHON_INSTALL(
    FILES
    gsm_bcch_ccch_demapper.py
    gsm_bcch_ccch_sdcch4_demapper.py
    gsm_sdcch8_demapper.py
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/gsm
)
