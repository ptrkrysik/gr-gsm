# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    gsm_burst_file_sink.block.yml
    gsm_burst_file_source.block.yml
    gsm_bursts_printer.block.yml
    gsm_clock_offset_corrector_tagged.block.yml
    gsm_controlled_fractional_resampler_cc.block.yml
    gsm_controlled_rotator_cc.block.yml
    gsm_extract_assignment_cmd.block.yml
    gsm_extract_immediate_assignment.block.yml
    gsm_extract_system_info.block.yml
    gsm_message_file_sink.block.yml
    gsm_message_file_source.block.yml
    gsm_message_printer.block.yml
    gsm_msg_to_tag.block.yml
    DESTINATION share/gnuradio/grc/blocks
)
