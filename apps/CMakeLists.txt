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

GR_PYTHON_INSTALL(
    PROGRAMS
    grgsm_airprobe.py
    grgsm_capture.py
    grgsm_decode.py
    grgsm_scanner.py
    gsm_channelize.py
    DESTINATION bin
)

#create logical links in order to keep legacy names of apps
macro(CREATE_SYMLINK _source _dest)
    set(source ${CMAKE_CURRENT_SOURCE_DIR}/${_source})
    set(dest ${CMAKE_CURRENT_BINARY_DIR}/${_dest})
    list(APPEND symlinks ${dest})
    add_custom_command(
        DEPENDS ${source} OUTPUT ${dest}
        COMMAND ln -sf ${_source} ${_dest}
    )
endmacro(CREATE_SYMLINK)

CREATE_SYMLINK(grgsm_decode.py airprobe_decode.py)
CREATE_SYMLINK(grgsm_capture.py airprobe_rtlsdr_capture.py)
CREATE_SYMLINK(grgsm_airprobe.py airprobe_rtlsdr.py)
CREATE_SYMLINK(grgsm_scanner.py airprobe_rtlsdr_scanner.py)

add_custom_target(multi_rtl_grc_xml_blocks ALL DEPENDS ${symlinks})

install(
    PROGRAMS
    ${symlinks}
    DESTINATION bin
)
