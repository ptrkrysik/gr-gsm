# Copyright 2024 Piotr Krysik <ptrkrysik@gmail.com>
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Install public header files
########################################################################
install(
    FILES
    burst_sink.h
    burst_source.h
    message_source.h
    message_sink.h
    DESTINATION include/gsm/qa_utils
)
