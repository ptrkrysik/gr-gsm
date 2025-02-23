# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    gsm_control_channels_decoder.block.yml
    gsm_tch_f_decoder.block.yml
    gsm_tch_h_decoder.block.yml
    DESTINATION share/gnuradio/grc/blocks
)
