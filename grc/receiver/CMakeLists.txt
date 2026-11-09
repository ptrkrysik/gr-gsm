# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    gsm_cx_channel_hopper.block.yml
    gsm_receiver.block.yml
    gsm_receiver_with_uplink.block.yml
    gsm_input.block.yml
    gsm_clock_offset_control.block.yml
    gsm_fcch_burst_tagger.block.yml
    gsm_fcch_detector.block.yml
    gsm_sch_detector.block.yml
    DESTINATION share/gnuradio/grc/blocks
)
