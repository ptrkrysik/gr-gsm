# This file is a part of gr-gsm
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

# The examples are plain GRC flowgraphs and python scripts - install them as is
install(DIRECTORY
    experimental_receiver_components_test
    frequency_hopping
    get_bcch_msgs_from_cfile
    voice_decoding_in_grc
    uplink_decoding_demo
    wideband_processing_demo
    DESTINATION share/gnuradio/examples/gsm
    USE_SOURCE_PERMISSIONS
    PATTERN "__pycache__" EXCLUDE
    PATTERN "*.pyc" EXCLUDE
)
install(FILES README.md DESTINATION share/gnuradio/examples/gsm)
