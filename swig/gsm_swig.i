/* -*- c++ -*- */

#define GSM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gsm_swig_doc.i"

%{
#include "gsm/receiver.h"
#include "gsm/bursts_printer.h"
#include "gsm/get_ccch_bursts.h"
%}


%include "gsm/receiver.h"
GR_SWIG_BLOCK_MAGIC2(gsm, receiver);
%include "gsm/bursts_printer.h"
GR_SWIG_BLOCK_MAGIC2(gsm, bursts_printer);
%include "gsm/get_ccch_bursts.h"
GR_SWIG_BLOCK_MAGIC2(gsm, get_ccch_bursts);
