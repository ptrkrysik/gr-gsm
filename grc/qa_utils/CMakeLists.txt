# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    gsm_burst_source.block.yml
    gsm_burst_sink.block.yml
    gsm_message_source.block.yml
    gsm_message_sink.block.yml
    DESTINATION share/gnuradio/grc/blocks
)
