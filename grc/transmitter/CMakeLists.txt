# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    gsm_gen_test_ab.block.yml
    gsm_gmsk_mod.block.yml
    gsm_preprocess_tx_burst.block.yml
    gsm_txtime_bursts_tagger.block.yml
    gsm_txtime_setter.block.yml
    DESTINATION share/gnuradio/grc/blocks
)
