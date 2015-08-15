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

include(GrPython)

SET(APPS_DIR ${CMAKE_SOURCE_DIR}/apps)

SET(GRCC_COMMAND ${PC_GNURADIO_RUNTIME_PREFIX}/${GR_RUNTIME_DIR}/grcc)
SET(BLOCK_PATH ../grc/)

ADD_CUSTOM_TARGET(airprobe_file.py ALL)
ADD_CUSTOM_COMMAND(
 TARGET airprobe_file.py
 COMMAND GRC_BLOCKS_PATH=${BLOCK_PATH} ${GRCC_COMMAND} -d ${CMAKE_CURRENT_BINARY_DIR} 
 ARGS "airprobe_file.grc"
# COMMAND ${GRCC_COMMAND} -d ${CMAKE_CURRENT_BINARY_DIR} 
 WORKING_DIRECTORY ${APPS_DIR}
)

ADD_CUSTOM_TARGET(airprobe_rtlsdr.py ALL)
ADD_CUSTOM_COMMAND(
 TARGET airprobe_rtlsdr.py
 COMMAND GRC_BLOCKS_PATH=${BLOCK_PATH} ${GRCC_COMMAND} -d ${CMAKE_CURRENT_BINARY_DIR} 
 ARGS "airprobe_rtlsdr.grc"
 WORKING_DIRECTORY ${APPS_DIR}
)

GR_PYTHON_INSTALL(
    airprobe_file.py
    airprobe_rtlsdr.py
    DESTINATION bin
)
