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
include(GrMiscUtils)

########################################################################
# Handle the generated constants
########################################################################
execute_process(COMMAND ${PYTHON_EXECUTABLE} -c
    "import time;print time.strftime('%a, %d %b %Y %H:%M:%S', time.gmtime())"
    OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Loading build date ${BUILD_DATE} into constants...")
message(STATUS "Loading version ${VERSION} into constants...")

#double escape for windows backslash path separators
string(REPLACE "\\" "\\\\" prefix "${prefix}")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/constants.cc.in
    ${CMAKE_CURRENT_BINARY_DIR}/constants.cc
    ESCAPE_QUOTES
@ONLY)

list(APPEND grgsm_sources ${CMAKE_CURRENT_BINARY_DIR}/constants.cc)
#########################################################################

include_directories(${Boost_INCLUDE_DIR} receiver)
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND grgsm_sources
    receiver/receiver_impl.cc
    receiver/receiver_config.cc
    receiver/viterbi_detector.cc
    decoding/sch.c
    receiver/clock_offset_control_impl.cc
    receiver/cx_channel_hopper_impl.cc
    demapping/universal_ctrl_chans_demapper_impl.cc
    demapping/tch_f_chans_demapper_impl.cc
    decoding/control_channels_decoder_impl.cc
    decoding/tch_f_decoder_impl.cc
    decoding/openbts/AmrCoder.cpp
    decoding/openbts/BitVector.cpp
    decoding/openbts/GSM610Tables.cpp
    decoding/openbts/GSM660Tables.cpp
    decoding/openbts/GSM503Tables.cpp
    decoding/openbts/ViterbiR204.cpp
    decoding/osmocom/coding/gsm0503_conv.c
    decoding/osmocom/coding/gsm0503_coding.c
    decoding/osmocom/coding/gsm0503_interleaving.c
    decoding/osmocom/coding/gsm0503_mapping.c
    decoding/osmocom/coding/gsm0503_parity.c
    decoding/osmocom/coding/gsm0503_tables.c
    flow_control/burst_timeslot_splitter_impl.cc
    flow_control/burst_sdcch_subslot_splitter_impl.cc
    flow_control/burst_timeslot_filter_impl.cc
    flow_control/burst_sdcch_subslot_filter_impl.cc
    flow_control/burst_fnr_filter_impl.cc
    flow_control/dummy_burst_filter_impl.cc
    flow_control/uplink_downlink_splitter_impl.cc
    misc_utils/collect_system_info_impl.cc
    misc_utils/controlled_rotator_cc_impl.cc
    misc_utils/controlled_fractional_resampler_cc_impl.cc
    misc_utils/msg_to_tag_impl.cc
    misc_utils/message_printer_impl.cc
    misc_utils/tmsi_dumper_impl.cc
    misc_utils/burst_file_sink_impl.cc
    misc_utils/burst_file_source_impl.cc
    misc_utils/message_file_sink_impl.cc
    misc_utils/message_file_source_impl.cc
    misc_utils/bursts_printer_impl.cc
    misc_utils/extract_system_info_impl.cc
    misc_utils/extract_immediate_assignment_impl.cc
    misc_utils/extract_cmc_impl.cc
    qa_utils/burst_sink_impl.cc
    qa_utils/burst_source_impl.cc
    qa_utils/message_source_impl.cc
    qa_utils/message_sink_impl.cc
    decryption/decryption_impl.cc
)


add_library(grgsm SHARED ${grgsm_sources})
target_link_libraries(grgsm ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES} ${VOLK_LIBRARIES} ${LIBOSMOCODEC_LIBRARIES} ${LIBOSMOCORE_LIBRARIES} 
# ${LIBOSMOCODING_LIBRARIES} 
# libraries required by plotting.h - have troubles to be installed by pybombs
#    boost_iostreams
#    boost_system
#    boost_filesystem
)
set_target_properties(grgsm PROPERTIES DEFINE_SYMBOL "grgsm_EXPORTS")
GR_LIBRARY_FOO(grgsm)

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
