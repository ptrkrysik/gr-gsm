# Continuous integration

GitHub Actions (`.github/workflows/ci.yml`) replaces the historical Travis CI
pipeline. It builds and installs gr-gsm in **Ubuntu 24.04 and Ubuntu 26.04**
containers, using each distribution's GNU Radio and Osmocom packages. Both jobs
also check the installed Python bindings, compile the livemon flowgraph with
`grcc`, and run the file-based integration test. No SDR or display is required.

The workflow runs on pushes, pull requests, and manual dispatch. A failed job
uploads available CMake and integration-test diagnostics.

## File-based integration test

After building and installing gr-gsm (including running `ldconfig`), run:

```sh
timeout 120s /usr/bin/python3 tests/integration/decode_capture.py \
  --download --capture build/integration/capture.cfile
```

To reuse a previously downloaded recording, omit `--download`.

The test downloads the first 6,000,000 bytes of
`vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile` from the
[`ptrkrysik/test_data`](https://github.com/ptrkrysik/test_data) repository, pinned
to commit `e2e5b44bc2906d5f01944355ce5c83b39eec6fee`. It checks the prefix's
SHA-256 before decoding.

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

The test constructs the flowgraph directly because `grgsm_decode` still depends
on flow-control blocks not yet ported to this branch. A download error, invalid
recording, missing reference frame, changed payload, missing/extra message, or
timeout fails the job. It does not require USB access or bind a UDP port.
