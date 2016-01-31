#!/bin/bash
export AP_DECODE=/usr/local/bin/airprobe_decode.py
export CAPFILE=/test_data/vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile
# We expect $CAPFILE to be defined.  If not, exit...

/usr/bin/python $AP_DECODE -c $CAPFILE
