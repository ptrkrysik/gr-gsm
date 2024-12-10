# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    gsm_universal_ctrl_chans_demapper.block.yml
    gsm_bcch_ccch_demapper.block.yml
    gsm_bcch_ccch_sdcch4_demapper.block.yml
    gsm_sdcch8_demapper.block.yml
    gsm_tch_f_chans_demapper.block.yml
    gsm_tch_h_chans_demapper.block.yml
    DESTINATION share/gnuradio/grc/blocks
)
