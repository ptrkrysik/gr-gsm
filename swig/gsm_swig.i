/* -*- c++ -*- */

#define GSM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gsm_swig_doc.i"

%{
#include "gsm/receiver.h"
#include "gsm/bursts_printer.h"
#include "gsm/get_bcch_or_ccch_bursts.h"
#include "gsm/control_channels_decoder.h"
#include "gsm/extract_system_info.h"
#include "gsm/controlled_rotator_cc.h"
#include "gsm/controlled_const_source_f.h"
#include "gsm/message_printer.h"
#include "gsm/wireshark_sink.h"
%}


%include "gsm/receiver.h"
GR_SWIG_BLOCK_MAGIC2(gsm, receiver);
%include "gsm/bursts_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, bursts_printer);

%include "gsm/get_bcch_or_ccch_bursts.h"
GR_SWIG_BLOCK_MAGIC2(gsm, get_bcch_or_ccch_bursts);
%include "gsm/control_channels_decoder.h"
GR_SWIG_BLOCK_MAGIC2(gsm, control_channels_decoder);
%include "gsm/extract_system_info.h"
GR_SWIG_BLOCK_MAGIC2(gsm, extract_system_info);
%include "gsm/controlled_rotator_cc.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_rotator_cc);
%include "gsm/controlled_const_source_f.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_const_source_f);
%include "gsm/message_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_printer);
%include "gsm/wireshark_sink.h"
GR_SWIG_BLOCK_MAGIC2(gsm, wireshark_sink);
