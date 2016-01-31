#!/bin/bash
export AP_DECODE=/usr/local/bin/airprobe_decode.py
export CAPFILE=/src/test_data/vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile

export RUNLINE="$AP_DECODE -c $CAPFILE -s $((100000000/174)) -a 725 -m BCCH -t 0"
echo "Testing with:\n  $RUNLINE"
$RUNLINE
