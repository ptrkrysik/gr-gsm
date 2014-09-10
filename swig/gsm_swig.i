/* -*- c++ -*- */

#define GSM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gsm_swig_doc.i"

%{
#include "gsm/receiver/receiver.h"
#include "gsm/demapping/get_bcch_or_ccch_bursts.h"
#include "gsm/decoding/control_channels_decoder.h"
#include "gsm/misc_utils/bursts_printer.h"
#include "gsm/misc_utils/controlled_const_source_f.h"
#include "gsm/misc_utils/controlled_rotator_cc.h"
#include "gsm/misc_utils/extract_system_info.h"
#include "gsm/misc_utils/message_printer.h"
#include "gsm/misc_utils/wireshark_sink.h"
%}


%include "gsm/receiver/receiver.h"
GR_SWIG_BLOCK_MAGIC2(gsm, receiver);

%include "gsm/decoding/control_channels_decoder.h"
GR_SWIG_BLOCK_MAGIC2(gsm, control_channels_decoder);

%include "gsm/demapping/get_bcch_or_ccch_bursts.h"
GR_SWIG_BLOCK_MAGIC2(gsm, get_bcch_or_ccch_bursts);

%include "gsm/misc_utils/bursts_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, bursts_printer);
%include "gsm/misc_utils/extract_system_info.h"
GR_SWIG_BLOCK_MAGIC2(gsm, extract_system_info);
%include "gsm/misc_utils/controlled_rotator_cc.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_rotator_cc);
%include "gsm/misc_utils/controlled_const_source_f.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_const_source_f);
%include "gsm/misc_utils/message_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_printer);
%include "gsm/misc_utils/wireshark_sink.h"
GR_SWIG_BLOCK_MAGIC2(gsm, wireshark_sink);
