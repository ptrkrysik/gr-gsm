/* -*- c++ -*- */

#define GSM_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gsm_swig_doc.i"

%{
#include "gsm/receiver.h"
%}


%include "gsm/receiver.h"
GR_SWIG_BLOCK_MAGIC2(gsm, receiver);
