#!/bin/bash
export AP_DECODE="grgsm_decode"
export CAPFILE="vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile"
export TEST_DATA_DIRECTORY="/src/test_data/"
export TEST_CONTROL_DIRECTORY="/src/build_test/control"
export RUNLINE="$AP_DECODE -c $CAPFILE -s $((100000000/174)) -m BCCH -t 0 -v "
echo "Testing with:"
echo "  $RUNLINE"
cd $TEST_DATA_DIRECTORY
gnuradio-companion --version

$RUNLINE | tail -n +4 | tee grgsm_decode_test1_result
diff grgsm_decode_test1_result $TEST_CONTROL_DIRECTORY/grgsm_decode_test1_expected
TEST_RESULT=$?

rm grgsm_decode_test1_result
if [ $TEST_RESULT == 0 ]
then
  echo "   Result: PASSED"
  exit 0
else
  echo "   Result: FAILED"
  exit 1
fi
