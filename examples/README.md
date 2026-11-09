# gr-gsm examples

Example flowgraphs and scripts for the `gnuradio.gsm` module (GNU Radio 3.10).
They are ports of <https://github.com/ptrkrysik/examples> (written for GNU Radio
3.7 and the `grgsm` python module) to the current GRC YAML flowgraph format and
the `gsm` module. The original repository is licensed under GPL-3.0 (see its
`LICENSE` file), the same license as gr-gsm.

The `.grc` files are the sources of the examples. Open them in GNU Radio
Companion or compile them with `grcc`, for example:

    grcc -o /tmp/out get_bcch_msgs_from_cfile/get_bcch_msgs_from_cfile.grc

When gr-gsm is not installed system-wide point GRC at its blocks with
`GRC_BLOCKS_PATH=<gr-gsm>/grc` (and python at the module with `PYTHONPATH`).
Python files generated from a flowgraph are not kept in the repository, except
the two that other examples import (see below). After `cmake --install` the
examples are in `share/gnuradio/examples/gsm`.

Flowgraphs that read from a file need recordings that are not part of this
repository (they live in the `ptrkrysik/test_data` repository or have to be
recorded by yourself). Only `get_bcch_msgs_from_cfile` and
`wideband_processing_demo` (with `--no-record`) are meant to work with the
small public test recording straight away.

| Example | What it does | Needs |
| --- | --- | --- |
| [`get_bcch_msgs_from_cfile`](get_bcch_msgs_from_cfile) | Decodes BCCH/CCCH messages (timeslot 0 of C0) from a complex float recording and prints them; also sends them as GSMTAP to UDP port 4729 (Wireshark). | An IQ recording (`.cfile`), no hardware. |
| [`wideband_processing_demo`](wideband_processing_demo) | Records 3 s from an RTL-SDR, splits the band into 200 kHz channels with a polyphase channelizer, decodes BCCH on each of them and lists the found C0 carriers with MNC/LAC/CID/power. Contains the `receiver_with_decoder` and `wideband_receiver` hierarchical blocks. | RTL-SDR (or an existing recording with `--no-record`). |
| [`experimental_receiver_components_test`](experimental_receiver_components_test) | Qt GUI test of the experimental receiver blocks (`fcch_detector`, `sch_detector`, python, need scipy): shows the signal with FCCH/SCH detections tagged. | RTL-SDR, scipy. |
| [`frequency_hopping`](frequency_hopping) | Receivers for frequency hopping channels: `grgsm_hopping_example.grc` (4 channels, hopping SDCCH/8 decrypted with a known Kc), `airprobe_rtlsdr_multi.grc` (12 channels, control channels of a hopping channel). | Recordings of the individual channels. |
| [`uplink_decoding_demo`](uplink_decoding_demo) | Decodes SDCCH/8 downlink and uplink at the same time (`receiver_with_uplink` with a downlink and an uplink stream) with a known Kc. | Downlink and uplink recordings (`sms_multirtl_*_tail.cfile`). |
| [`voice_decoding_in_grc`](voice_decoding_in_grc) | Decodes a TCH/F GSM full rate voice channel (timeslot 5) of a recorded call, plays it on the sound card and writes the raw frames to `out.gsm`. | The full `vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile` recording, a sound card. |

The following examples of the original repository were not ported:
`transmitting_demo`, `trx_radio_if` and `trx_test` (transmitting side and the
TRX interface are not supported in this port).

Notes on the changes against the original repository:

* block ids and parameters were adapted (`blocks_socket_pdu` ->
  `network_socket_pdu`, `grgsm.*` -> `gsm.*`, `fractional_resampler_xx` ->
  `mmse_resampler_xx`, `blocks_pdu_to_tagged_stream` ->
  `pdu_pdu_to_tagged_stream`, `firdes.WIN_*` -> `window.WIN_*`),
* wx GUI elements were dropped,
* hard-coded absolute paths of the author's machine were replaced by
  parameters or relative paths.
