/* -*- c++ -*- */

#define GSM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "grgsm_swig_doc.i"

%{
#include "grgsm/receiver/receiver.h"
#include "grgsm/receiver/clock_offset_control.h"
#include "grgsm/receiver/cx_channel_hopper.h"
#include "grgsm/decoding/control_channels_decoder.h"
#include "grgsm/decoding/tch_f_decoder.h"
#include "grgsm/decryption/decryption.h"
#include "grgsm/demapping/universal_ctrl_chans_demapper.h"
#include "grgsm/demapping/tch_f_chans_demapper.h"
#include "grgsm/flow_control/burst_timeslot_splitter.h"
#include "grgsm/flow_control/burst_sdcch_subslot_splitter.h"
#include "grgsm/flow_control/burst_timeslot_filter.h"
#include "grgsm/flow_control/burst_sdcch_subslot_filter.h"
#include "grgsm/flow_control/burst_fnr_filter.h"
#include "grgsm/flow_control/dummy_burst_filter.h"
#include "grgsm/misc_utils/bursts_printer.h"
#include "grgsm/misc_utils/controlled_const_source_f.h"
#include "grgsm/misc_utils/controlled_rotator_cc.h"
#include "grgsm/misc_utils/extract_system_info.h"
#include "grgsm/misc_utils/extract_immediate_assignment.h"
#include "grgsm/misc_utils/message_printer.h"
#include "grgsm/misc_utils/tmsi_dumper.h"
#include "grgsm/misc_utils/burst_file_sink.h"
#include "grgsm/misc_utils/burst_file_source.h"
#include "grgsm/qa_utils/burst_sink.h"
#include "grgsm/qa_utils/burst_source.h"
#include "grgsm/qa_utils/message_source.h"
#include "grgsm/qa_utils/message_sink.h"
#include "grgsm/misc_utils/message_file_sink.h"
#include "grgsm/misc_utils/message_file_source.h"
%}

%include "grgsm/receiver/receiver.h"
GR_SWIG_BLOCK_MAGIC2(gsm, receiver);
%include "grgsm/receiver/clock_offset_control.h"
GR_SWIG_BLOCK_MAGIC2(gsm, clock_offset_control);
%include "grgsm/receiver/cx_channel_hopper.h"
GR_SWIG_BLOCK_MAGIC2(gsm, cx_channel_hopper);

%include "grgsm/decoding/control_channels_decoder.h"
GR_SWIG_BLOCK_MAGIC2(gsm, control_channels_decoder);
%include "grgsm/decoding/tch_f_decoder.h"
GR_SWIG_BLOCK_MAGIC2(gsm, tch_f_decoder);

%include "grgsm/decryption/decryption.h"
GR_SWIG_BLOCK_MAGIC2(gsm, decryption);

%include "grgsm/demapping/universal_ctrl_chans_demapper.h"
GR_SWIG_BLOCK_MAGIC2(gsm, universal_ctrl_chans_demapper);
%include "grgsm/demapping/tch_f_chans_demapper.h"
GR_SWIG_BLOCK_MAGIC2(gsm, tch_f_chans_demapper);

%include "grgsm/flow_control/burst_timeslot_splitter.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_timeslot_splitter);
%include "grgsm/flow_control/burst_sdcch_subslot_splitter.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_sdcch_subslot_splitter);
%include "grgsm/flow_control/burst_timeslot_filter.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_timeslot_filter);
%include "grgsm/flow_control/burst_sdcch_subslot_filter.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_sdcch_subslot_filter);
%include "grgsm/flow_control/burst_fnr_filter.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_fnr_filter);
%include "grgsm/flow_control/dummy_burst_filter.h"
GR_SWIG_BLOCK_MAGIC2(gsm, dummy_burst_filter);

%include "grgsm/misc_utils/bursts_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, bursts_printer);
%include "grgsm/misc_utils/burst_file_sink.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_file_sink);
%include "grgsm/misc_utils/burst_file_source.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_file_source);
%include "grgsm/misc_utils/extract_system_info.h"
GR_SWIG_BLOCK_MAGIC2(gsm, extract_system_info);
%include "grgsm/misc_utils/extract_immediate_assignment.h"
GR_SWIG_BLOCK_MAGIC2(gsm, extract_immediate_assignment);
%include "grgsm/misc_utils/controlled_rotator_cc.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_rotator_cc);
%include "grgsm/misc_utils/controlled_const_source_f.h"
GR_SWIG_BLOCK_MAGIC2(gsm, controlled_const_source_f);
%include "grgsm/misc_utils/message_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_printer);
%include "grgsm/misc_utils/tmsi_dumper.h"
GR_SWIG_BLOCK_MAGIC2(gsm, tmsi_dumper);
%include "grgsm/misc_utils/message_file_sink.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_file_sink);
%include "grgsm/misc_utils/message_file_source.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_file_source);

%include "grgsm/qa_utils/burst_sink.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_sink);
%include "grgsm/qa_utils/burst_source.h"
GR_SWIG_BLOCK_MAGIC2(gsm, burst_source);
%include "grgsm/qa_utils/message_source.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_source);
%include "grgsm/qa_utils/message_sink.h"
GR_SWIG_BLOCK_MAGIC2(gsm, message_sink);
