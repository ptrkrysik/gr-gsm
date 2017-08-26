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
include(CreateSymlink)
add_subdirectory(helpers)

GR_PYTHON_INSTALL(
    PROGRAMS
    grgsm_livemon
    grgsm_scanner
    grgsm_decode
    DESTINATION bin
)

SET(XDG_DESKTOP_DIR "/usr/share/applications/")
INSTALL (FILES grgsm_livemon.desktop DESTINATION ${XDG_DESKTOP_DIR})

#CREATE_SYMLINK(grgsm_livemon airprobe_rtlsdr.py)
#CREATE_SYMLINK(grgsm_decode airprobe_decode.py)
#CREATE_SYMLINK(grgsm_scanner airprobe_rtlsdr_scanner.py)

add_custom_target(apps_symlinks ALL DEPENDS ${symlinks})

install(
    PROGRAMS
    ${symlinks}
    DESTINATION bin
)

