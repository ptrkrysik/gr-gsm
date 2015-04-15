/* -*- c++ -*- */

#define GSM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "grgsm_swig_doc.i"

%{
#include "grgsm/receiver/receiver.h"
#include "grgsm/receiver/clock_offset_control.h"
#include "grgsm/decoding/control_channels_decoder.h"
#include "grgsm/decryption/decryption.h"
#include "grgsm/demapping/universal_ctrl_chans_demapper.h"
#include "grgsm/demapping/tch_f_chans_demapper.h"
#include "grgsm/misc_utils/bursts_printer.h"
#include "grgsm/misc_utils/controlled_const_source_f.h"
#include "grgsm/misc_utils/controlled_rotator_cc.h"
#include "grgsm/misc_utils/extract_system_info.h"
#include "grgsm/misc_utils/message_printer.h"
#include "grgsm/misc_utils/tmsi_dumper.h"
%}


%include "grgsm/receiver/receiver.h"
GR_SWIG_BLOCK_MAGIC2(gsm, receiver);
%include "grgsm/receiver/clock_offset_control.h"
GR_SWIG_BLOCK_MAGIC2(gsm, clock_offset_control);


%include "grgsm/decoding/control_channels_decoder.h"
GR_SWIG_BLOCK_MAGIC2(gsm, control_channels_decoder);

%include "grgsm/decryption/decryption.h"
GR_SWIG_BLOCK_MAGIC2(gsm, decryption);

%include "grgsm/demapping/universal_ctrl_chans_demapper.h"
GR_SWIG_BLOCK_MAGIC2(gsm, universal_ctrl_chans_demapper);
%include "grgsm/demapping/tch_f_chans_demapper.h"
GR_SWIG_BLOCK_MAGIC2(gsm, tch_f_chans_demapper);

%include "grgsm/misc_utils/bursts_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, bursts_printer);
%include "grgsm/misc_utils/extract_system_info.h"
GR_SWIG_BLOCK_MAGIC2(gsm, extract_system_info);
%include "grgsm/misc_utils/controlled_rotator_cc.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_rotator_cc);
%include "grgsm/misc_utils/controlled_const_source_f.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_const_source_f);
%include "grgsm/misc_utils/message_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_printer);
%include "grgsm/misc_utils/tmsi_dumper.h"
GR_SWIG_BLOCK_MAGIC2(gsm, tmsi_dumper);

