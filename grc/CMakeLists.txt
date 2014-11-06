# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    gsm_block_tree.xml
    receiver/gsm_input.xml
    receiver/gsm_receiver.xml
    misc_utils/gsm_bursts_printer.xml
    receiver/gsm_fcch_burst_tagger.xml
    receiver/gsm_sch_detector.xml
    receiver/gsm_fcch_detector.xml
    demapping/gsm_get_bcch_or_ccch_bursts.xml
    decoding/gsm_control_channels_decoder.xml
    misc_utils/gsm_extract_system_info.xml
    misc_utils/gsm_controlled_rotator_cc.xml
    misc_utils/gsm_controlled_const_source_f.xml
    receiver/gsm_clock_offset_control.xml
    misc_utils/gsm_message_printer.xml
    misc_utils/gsm_clock_offset_corrector.xml DESTINATION share/gnuradio/grc/blocks
)
