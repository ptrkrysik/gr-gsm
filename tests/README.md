# Continuous integration

GitHub Actions (`.github/workflows/ci.yml`) replaces the historical Travis CI
pipeline. It builds and installs gr-gsm in **Ubuntu 24.04 and Ubuntu 26.04**
containers, using each distribution's GNU Radio and Osmocom packages. Both jobs
also check the installed Python bindings, compile the livemon flowgraph with
`grcc`, and run the file-based integration test. No SDR or display is required.

The workflow runs on pushes, pull requests, and manual dispatch. A failed job
uploads available CMake and integration-test diagnostics.

A second workflow (`.github/workflows/other-distros.yml`) builds gr-gsm with the
packages of **Ubuntu 22.04, Debian 12 and Debian 13**, runs the unit tests, installs,
checks the Python module and the installed `grgsm_decode`, and uninstalls. The
dependency list it installs is the one given in the README.

## Test data

The integration tests use a real recording of a call from the repository
[`ptrkrysik/test_data`](https://github.com/ptrkrysik/test_data) (ARFCN 725, decimation 174,
the A5/1 key of the call is known). The data used to be a git submodule. Now
`tests/integration/test_data.py` downloads it when it is needed, from the commit
`e2e5b44bc2906d5f01944355ce5c83b39eec6fee`, and checks the SHA-256 of every file. Files that are
already in the destination directory and match are not downloaded again, so the directory
can be cached (CI does it with `actions/cache`):

```sh
python3 tests/integration/test_data.py build/test_data
```

It fetches the IQ file of the call (73 MB), its burst file (235 KB) and the two recordings of an SMS
(32 MB each), and prepares the two prefixes of the IQ file and the unpacked burst file that the
tests use. Nothing but a network connection is
needed, no SDR and no display.

## Integration tests

After building and installing gr-gsm (including running `ldconfig`) and fetching the data, run:

```sh
python3 tests/integration/decode_capture.py --capture build/test_data/prefix_6000000.cfile
python3 tests/integration/cli_tools.py --capture build/test_data/prefix_6000000.cfile
python3 tests/integration/reference_call.py --data-dir build/test_data
python3 tests/integration/sms_call.py --data-dir build/test_data
```

* `decode_capture.py` runs a flowgraph built in Python, see below.
* `cli_tools.py` runs the installed programs: `--help` of all of them, `grgsm_decode` on the 6 MB
  prefix against the reference BCCH messages, and `grgsm_livemon_headless` fed from the file, which
  has to send GSMTAP packets over UDP.
* `reference_call.py` runs `grgsm_decode` on the whole call, replaces the historical Travis scripts
  `decode.sh` and `decrypt.sh` and adds two checks:
  * the BCCH messages decoded from the **burst file** contain the 47 reference messages
    (this also tests reading burst files),
  * **SDCCH8 with the A5/1 key** decrypts to the 12 reference messages of the former
    `decrypt.sh` (`grgsm_decode_decrypt1_expected`), using the first 35.8 MB of the recording,
  * **TCH/F voice**: exactly 389 valid GSM-FR speech frames (about 7.8 s) are decoded from the whole
    recording.

* `sms_call.py` decodes a **short message** from the downlink and the uplink recordings of an SDCCH/8
  channel (`sms_multirtl_downlink_tail.cfile` and `sms_multirtl_uplink_tail.cfile`, recorded with two
  RTL-SDR dongles while a phone sent an SMS). It uses the flowgraph of `examples/uplink_decoding_demo`:
  two inputs, a receiver that processes the uplink, the SDCCH/8 demapper, A5/1 decryption with the
  known key and the control channel decoder. The test reassembles the three LAPDm segments of the
  SMS-SUBMIT (`sms_decode.py`) and checks the destination number (530316008), the text
  (`Test sms 12345678910111213141516k`) and that the network acknowledged the message with an RP-ACK
  of the same reference. It covers the uplink receiver and the decryption of the uplink, which
  nothing else tests. The clock correction of the dongles has to be given (`-10 ppm`): without it
  the last segment of the message is lost.

The decoding is deterministic: the voice output is identical between runs and between the generic and
the SIMD VOLK kernels.

## The flowgraph test

`decode_capture.py` downloads nothing when it is given an existing file; `--download` fetches the
6 MB prefix by itself, which is handy for a quick local run:

```sh
timeout 120s /usr/bin/python3 tests/integration/decode_capture.py \
  --download --capture build/integration/capture.cfile
```

The signal path is:

```text
IQ file -> gsm_input -> receiver -> BCCH/CCCH demapper
        -> control_channels_decoder -> message collector
```

Sample rate (`100000000 // 174`), PPM correction (`-10`), recording length,
and all 47 reference messages come from the former Travis `decode.sh` test
and its `grgsm_decode_test1_expected` fixture. `bcch_expected.txt` retains
the GSM frame numbers and all 23 payload bytes; the redundant printed
frame-count column is omitted. Messages before the historical acquisition
anchor are ignored, as in the old test. From that anchor onward, the complete
sequence must match, including its length and order.

The test constructs the flowgraph directly instead of running `grgsm_decode`. A download error, invalid
recording, missing reference frame, changed payload, missing/extra message, or
timeout fails the job. It does not require USB access or bind a UDP port.
