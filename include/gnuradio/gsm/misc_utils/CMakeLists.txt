# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

install(
    FILES
    burst_file_sink.h
    burst_file_source.h
    bursts_printer.h
    controlled_fractional_resampler_cc.h
    controlled_rotator_cc.h
    extract_assignment_cmd.h
    extract_immediate_assignment.h
    message_file_sink.h
    message_file_source.h
    message_printer.h
    msg_to_tag.h
    time_spec.h
    tmsi_dumper.h
    DESTINATION include/gnuradio/gsm/misc_utils
)
