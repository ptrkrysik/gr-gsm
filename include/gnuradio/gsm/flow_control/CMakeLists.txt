# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    burst_fnr_filter.h
    burst_sdcch_subslot_filter.h
    burst_sdcch_subslot_splitter.h
    burst_timeslot_filter.h
    burst_timeslot_splitter.h
    burst_type_filter.h
    common.h
    dummy_burst_filter.h
    uplink_downlink_splitter.h
    DESTINATION include/gnuradio/gsm/flow_control
)
