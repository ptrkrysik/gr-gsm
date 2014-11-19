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

include_directories(${Boost_INCLUDE_DIR} receiver burst_printer)
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND gsm_sources
    receiver/receiver_impl.cc
    receiver/receiver_config.cc 
    receiver/viterbi_detector.cc 
    receiver/sch.c
    misc_utils/bursts_printer_impl.cc
    misc_utils/extract_system_info_impl.cc
    demapping/universal_ctrl_chans_demapper_impl.cc
    demapping/get_bcch_or_ccch_bursts_impl.cc
    decoding/control_channels_decoder_impl.cc
    decoding/cch.c
    decoding/fire_crc.c
    misc_utils/controlled_rotator_cc_impl.cc
    misc_utils/controlled_const_source_f_impl.cc
    misc_utils/message_printer_impl.cc
)

add_library(gnuradio-gsm SHARED ${gsm_sources})
target_link_libraries(gnuradio-gsm ${Boost_LIBRARIES} ${GNURADIO_RUNTIME_LIBRARIES}  
    boost_iostreams
    boost_system
    boost_filesystem
)
set_target_properties(gnuradio-gsm PROPERTIES DEFINE_SYMBOL "gnuradio_gsm_EXPORTS")

########################################################################
# Install built library files
########################################################################
install(TARGETS gnuradio-gsm
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)

########################################################################
# Build and register unit test
########################################################################
#include(GrTest)

#include_directories(${CPPUNIT_INCLUDE_DIRS})

#list(APPEND test_gsm_sources
#    ${CMAKE_CURRENT_SOURCE_DIR}/test_gsm.cc
#    ${CMAKE_CURRENT_SOURCE_DIR}/qa_gsm.cc
#    ${CMAKE_CURRENT_SOURCE_DIR}/qa_receiver.cc
#)

#add_executable(test-gsm ${test_gsm_sources})

#target_link_libraries(
#  test-gsm
#  ${GNURADIO_RUNTIME_LIBRARIES}
#  ${Boost_LIBRARIES}
#  ${CPPUNIT_LIBRARIES}
#  gnuradio-gsm
#)

#GR_ADD_TEST(test_gsm test-gsm)
