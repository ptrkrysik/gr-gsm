# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT gsm_sources)
    message(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT gsm_sources)

########################################################################
# Check for pygccxml
########################################################################
gr_python_check_module_raw("pygccxml" "import pygccxml" PYGCCXML_FOUND)

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND gsm_python_files
    control_channels_decoder_python.cc
    receiver_python.cc
    tch_f_chans_demapper_python.cc
    tch_f_decoder_python.cc
    tch_h_chans_demapper_python.cc
    tch_h_decoder_python.cc
    universal_ctrl_chans_demapper_python.cc
    python_bindings.cc)

gr_pybind_make_oot(gsm ../../.. gr::gsm "${gsm_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(
    TARGET gsm_python
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:gsm_python>
            ${PROJECT_BINARY_DIR}/test_modules/gnuradio/gsm/)

install(
    TARGETS gsm_python
    DESTINATION ${GR_PYTHON_DIR}/gnuradio/gsm
    COMPONENT pythonapi)
