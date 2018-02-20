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
add_subdirectory(helpers)
add_subdirectory(apps_data)

SET(PYTHONPATH
	${CMAKE_SOURCE_DIR}/python
	${CMAKE_SOURCE_DIR}/python/misc_utils
	${CMAKE_SOURCE_DIR}/python/demapping
	${CMAKE_SOURCE_DIR}/python/receiver
	${CMAKE_BINARY_DIR}/swig
	$ENV{PYTHONPATH}
	)
string(REPLACE ";" ":" PYTHONPATH "${PYTHONPATH}")

ADD_CUSTOM_COMMAND(
	OUTPUT grgsm_livemon
	COMMAND "${CMAKE_COMMAND}"
		-E env PYTHONPATH="${PYTHONPATH}" GRC_BLOCKS_PATH=${CMAKE_SOURCE_DIR}/grc
		${PC_GNURADIO_RUNTIME_PREFIX}/${GR_RUNTIME_DIR}/grcc -d ${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/grgsm_livemon.grc
	COMMAND "${CMAKE_COMMAND}" -E rename grgsm_livemon.py grgsm_livemon
	DEPENDS grgsm_livemon.grc
	)
ADD_CUSTOM_COMMAND(
	OUTPUT grgsm_livemon_headless
	COMMAND "${CMAKE_COMMAND}"
		-E env PYTHONPATH="${PYTHONPATH}" GRC_BLOCKS_PATH=${CMAKE_SOURCE_DIR}/grc
		${PC_GNURADIO_RUNTIME_PREFIX}/${GR_RUNTIME_DIR}/grcc -d ${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/grgsm_livemon_headless.grc
	COMMAND "${CMAKE_COMMAND}" -E rename grgsm_livemon_headless.py grgsm_livemon_headless
	DEPENDS grgsm_livemon_headless.grc
	)

GR_PYTHON_INSTALL(
    PROGRAMS
	${CMAKE_CURRENT_BINARY_DIR}/grgsm_livemon
	${CMAKE_CURRENT_BINARY_DIR}/grgsm_livemon_headless
    grgsm_scanner
    grgsm_decode
    DESTINATION bin
)

install(
    PROGRAMS
    DESTINATION bin
)
