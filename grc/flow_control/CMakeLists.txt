# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    gsm_burst_fnr_filter.block.yml
    gsm_burst_sdcch_subslot_filter.block.yml
    gsm_burst_sdcch_subslot_splitter.block.yml
    gsm_burst_timeslot_filter.block.yml
    gsm_burst_timeslot_splitter.block.yml
    gsm_burst_type_filter.block.yml
    gsm_dummy_burst_filter.block.yml
    gsm_uplink_downlink_splitter.block.yml
    DESTINATION share/gnuradio/grc/blocks
)
