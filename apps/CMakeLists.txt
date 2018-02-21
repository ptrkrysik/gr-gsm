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
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/grgsm_livemon
	COMMAND "${CMAKE_COMMAND}"
		-E env PYTHONPATH="${PYTHONPATH}" GRC_BLOCKS_PATH=${CMAKE_SOURCE_DIR}/grc
		${PC_GNURADIO_RUNTIME_PREFIX}/${GR_RUNTIME_DIR}/grcc -d ${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/grgsm_livemon.grc
	COMMAND "${CMAKE_COMMAND}" -E rename grgsm_livemon.py grgsm_livemon
	DEPENDS grgsm_livemon.grc
	)
ADD_CUSTOM_COMMAND(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/grgsm_livemon_headless
	COMMAND "${CMAKE_COMMAND}"
		-E env PYTHONPATH="${PYTHONPATH}" GRC_BLOCKS_PATH=${CMAKE_SOURCE_DIR}/grc
		${PC_GNURADIO_RUNTIME_PREFIX}/${GR_RUNTIME_DIR}/grcc -d ${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/grgsm_livemon_headless.grc
	COMMAND "${CMAKE_COMMAND}" -E rename grgsm_livemon_headless.py grgsm_livemon_headless
	DEPENDS grgsm_livemon_headless.grc
	)

########################################################################
# Override the GR_UNIQUE_TARGET function to not append a hash
# to the `target` name, because we need a known name in order
# to add an explicit dependency that's needed for the parallel build
#
# The original code segment (taken from GrPython.cmake) is
#
#    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import re, hashlib
#unique = hashlib.md5('${reldir}${ARGN}').hexdigest()[:5]
#print(re.sub('\\W', '_', '${desc} ${reldir} ' + unique))"
#    OUTPUT_VARIABLE _target OUTPUT_STRIP_TRAILING_WHITESPACE)
#
########################################################################
function(GR_UNIQUE_TARGET desc)
    file(RELATIVE_PATH reldir ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    execute_process(COMMAND ${PYTHON_EXECUTABLE} -c "import re, hashlib
print(re.sub('\\W', '_', '${desc} ${reldir}'))"
    OUTPUT_VARIABLE _target OUTPUT_STRIP_TRAILING_WHITESPACE)
    add_custom_target(${_target} ALL DEPENDS ${ARGN})
endfunction(GR_UNIQUE_TARGET)

GR_PYTHON_INSTALL(
    PROGRAMS
	${CMAKE_CURRENT_BINARY_DIR}/grgsm_livemon
	${CMAKE_CURRENT_BINARY_DIR}/grgsm_livemon_headless
    grgsm_scanner
    grgsm_decode
    DESTINATION bin
)

# The add_dependencies(...) is very important for the parallel build `make -j $(nproc)`
# The `pygen_apps` target is generated in GR_PYTHON_INSTALL function which calls
# GR_UNIQUE_TARGET that we redefine above.
add_dependencies(pygen_apps _grgsm_swig)

install(
    PROGRAMS
    DESTINATION bin
)
