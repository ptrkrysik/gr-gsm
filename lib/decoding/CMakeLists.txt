# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

add_subdirectory(openbts)

add_sources(
    control_channels_decoder_impl.cc
    tch_f_decoder_impl.cc
    tch_h_decoder_impl.cc
    sch.c
)
