#
# Copyright 2008,2009 Free Software Foundation, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

# The presence of this file turns this directory into a Python package

'''
This is the GNU Radio GSM module. Place your Python package
description here (python/__init__.py).
'''
import os

if "CMAKE_BINARY_DIR" in os.environ:
    dirname, filename = os.path.split(os.path.abspath(__file__))

    # As the directory structure in the repository is different then the one after the package
    # gets installed we need to add those subdirectories to the __path__ otherwise python3 is
    # not able to load the modules using the relative import syntax and grcc compilation and
    # some unit tests fail.
    __path__ += [
        # Load the local (not yet installed) python modules from the local subdirectories
        os.path.join(dirname, "demapping")]

# import pybind11 generated symbols into the gsm namespace
try:
    # this might fail if the module is python-only
    from .gsm_python import *
except ModuleNotFoundError:
    pass

# import any pure python here
#
try:
    # import any pure python here
    from .gsm_bcch_ccch_demapper import gsm_bcch_ccch_demapper
    from .gsm_bcch_ccch_sdcch4_demapper import gsm_bcch_ccch_sdcch4_demapper
    from .gsm_sdcch8_demapper import gsm_sdcch8_demapper
except ImportError as e:
    import traceback; traceback.print_exc()
    raise