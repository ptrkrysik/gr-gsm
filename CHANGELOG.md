# Changelog

## 1.0.1

### Added
- Packages: `dists/debian` builds `libgnuradio-gsm1.0.0`, `gr-gsm` and `gr-gsm-dev`
  (Python 3, the unit tests run during the build), `dists/arch/PKGBUILD` builds the Arch Linux
  package. CI builds both: Debian package on Ubuntu 24.04 and Debian 12 and 13, Arch Linux in
  an Arch container with libosmocore from the AUR.

### Fixed
- A serial build with CMake 4 (as `makepkg` does it) failed with "Is a directory" when the
  bindings were copied to a directory that did not exist yet.
- `qa_txtime_setter` depended on the order in which GNU Radio handles messages of different
  ports and failed under load.
- When `grcc` is missing, CMake says so (it used to fail with "no such file or directory").
  On Arch Linux `grcc` is in the package `gnuradio-companion`.

## 1.0.0

The first release that is a stable, maintained port to GNU Radio 3.10. The version of the
library is now 1.0.0 (v0.43.0 was the first release of the port and is superseded by it).
The cleaned up and reduced code base is planned as version 2.

### Supported
- GNU Radio 3.10 (pybind11, no SWIG). CI builds and tests on Ubuntu 22.04, 24.04, 26.04, and
  Debian 12 and 13. The library also builds against the development branch of GNU Radio.
- The previous code for GNU Radio 3.7/3.8 is on the branch `legacy/swig-gnuradio-3.8`.
- `grgsm_trx` and the OsmocomBB transceiver interface are not ported.

### Added
- Command line tools are installed: `grgsm_decode`, `grgsm_livemon`, `grgsm_livemon_headless`,
  `grgsm_scanner`, `grgsm_capture`, `grgsm_channelize`.
- Examples in `examples/` (installed into the share directory).
- Integration tests on the recordings of `ptrkrysik/test_data` (downloaded by a script, checked
  with SHA-256): BCCH, SDCCH8 with A5/1, TCH/F voice and an SMS from the downlink and the uplink.
- CI: build and tests on 5 systems, Debug build with AddressSanitizer and UndefinedBehaviorSanitizer,
  check of the installed headers, a weekly informational build against GNU Radio main.

### Fixed
- All public headers are installed into `include/gnuradio/gsm` (`misc_utils`, `decryption` and
  `clock_offset_control.h` were missing, `qa_utils` went to `include/gsm`).
- Wrong bit in the TSC1 training sequence (#593, #635).
- AMR full rate decoding read past the end of the coded block (#589).
- Reading burst files failed with `pmt::deserialize: malformed input stream` on GNU Radio before
  3.10.12 (#477, GNU Radio issue 7672).
- `grgsm_scanner` `--args` was ignored; `--if-gain` and `--bb-gain` added (#611).
- Out of bounds read in the `burst_source` test block and left shift of negative values in the
  AMR Viterbi decoders (found by the sanitizers).
- Configuring no longer needs `libsndfile1-dev` with GNU Radio 3.10.1.

### Removed
- Three empty test skeletons that checked nothing.
