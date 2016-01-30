#!/bin/bash
export AP_DECODE=/usr/local/bin/airprobe_decode.py
# We expect $CAPFILE to be defined.  If not, exit...
if test -z $CAPFILE ; then
  echo "Missing CAPFILE environment variable."
  exit 1
fi
/usr/bin/python $AP_DECODE -c $CAPFILE
