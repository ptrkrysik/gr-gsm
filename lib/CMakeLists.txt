# Copyright 2011,2012 Free Software Foundation, Inc.
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

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR} receiver)
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND grgsm_sources
    receiver/receiver_impl.cc
    receiver/receiver_config.cc
    receiver/viterbi_detector.cc
    receiver/sch.c
    receiver/clock_offset_control_impl.cc
    receiver/cx_channel_hopper_impl.cc
    misc_utils/bursts_printer_impl.cc
    misc_utils/extract_system_info_impl.cc
    misc_utils/extract_immediate_assignment_impl.cc
    demapping/universal_ctrl_chans_demapper_impl.cc
    demapping/tch_f_chans_demapper_impl.cc
    decoding/control_channels_decoder_impl.cc
    decoding/cch.c
    decoding/fire_crc.c
    decoding/tch_f_decoder_impl.cc
    decoding/AmrCoder.cpp
    decoding/BitVector.cpp
    decoding/GSM610Tables.cpp
    decoding/GSM660Tables.cpp
    decoding/GSM503Tables.cpp
    decoding/ViterbiR204.cpp
    flow_control/burst_timeslot_splitter_impl.cc
    flow_control/burst_sdcch_subslot_splitter_impl.cc
    flow_control/burst_timeslot_filter_impl.cc
    flow_control/burst_sdcch_subslot_filter_impl.cc
    flow_control/burst_fnr_filter_impl.cc
    flow_control/dummy_burst_filter_impl.cc
    misc_utils/controlled_rotator_cc_impl.cc
    misc_utils/controlled_const_source_f_impl.cc
    misc_utils/message_printer_impl.cc
    misc_utils/tmsi_dumper_impl.cc
    misc_utils/burst_file_sink_impl.cc
    misc_utils/burst_file_source_impl.cc
    misc_utils/message_file_sink_impl.cc
    misc_utils/message_file_source_impl.cc   
    qa_utils/burst_sink_impl.cc
    qa_utils/burst_source_impl.cc
    qa_utils/message_source_impl.cc
    qa_utils/message_sink_impl.cc
    decryption/decryption_impl.cc
)


add_library(grgsm SHARED ${grgsm_sources})
target_link_libraries(grgsm ${Boost_LIBRARIES} ${GNURADIO_RUNTIME_LIBRARIES} ${VOLK_LIBRARIES} ${LIBOSMOCORE_LIBRARIES}
# libraries required by plotting.h - have troubles to be installed by pybombs
#    boost_iostreams
#    boost_system
#    boost_filesystem
)
set_target_properties(grgsm PROPERTIES DEFINE_SYMBOL "grgsm_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS grgsm
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
#include(GrTest)

#include_directories(${CPPUNIT_INCLUDE_DIRS})

#list(APPEND test_grgsm_sources
#    ${CMAKE_CURRENT_SOURCE_DIR}/test_gsm.cc
#    ${CMAKE_CURRENT_SOURCE_DIR}/qa_gsm.cc
#    ${CMAKE_CURRENT_SOURCE_DIR}/qa_receiver.cc
#)

#add_executable(test-gsm ${test_grgsm_sources})

#target_link_libraries(
#  test-gsm
#  ${GNURADIO_RUNTIME_LIBRARIES}
#  ${Boost_LIBRARIES}
#  ${CPPUNIT_LIBRARIES}
#  grgsm
#)

#GR_ADD_TEST(test_gsm test-gsm)
