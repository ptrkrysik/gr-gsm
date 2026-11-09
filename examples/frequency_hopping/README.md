Receivers of frequency hopping channels. `gsm_cx_channel_hopper` follows the
hopping sequence (MA, MAIO, HSN) over the CX streams of `gsm_receiver`, so one
receiver block is fed with a stream per ARFCN of the mobile allocation.

* `grgsm_hopping_example.grc` - four recordings `out_<ARFCN>.cfile` (ARFCNs 51,
  2, 37, 45, one channel per file, 0.5 MHz sampling rate by default) in the
  directory `recording9_hopping_downlink_941M_samp_12.5M_20160226`; the
  hopping SDCCH/8 of timeslot 0 (MA 2, 37, 45) is followed and decrypted with a
  known Kc. A TCH/F branch and a BCCH branch are present but disabled.
* `airprobe_rtlsdr_multi.grc` - twelve per-channel recordings `out_<N>s.cfile`
  in one directory (parameter `-d`), the universal control channels demapper
  follows a hopping channel with MA 990-995, 1019-1023, HSN 3, MAIO 2.

Both examples need recordings that are not part of the repository. The
flowgraphs compile to python scripts with `grcc`.
