#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Decode the historical Travis IQ recording and compare BCCH/CCCH messages."""

import argparse
import difflib
import hashlib
from pathlib import Path
import struct
import urllib.request

from gnuradio import blocks, gr, gsm
import pmt


CAPTURE_COMMIT = "e2e5b44bc2906d5f01944355ce5c83b39eec6fee"
CAPTURE_NAME = "vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile"
CAPTURE_URL = (
    f"https://raw.githubusercontent.com/ptrkrysik/test_data/{CAPTURE_COMMIT}/{CAPTURE_NAME}"
)
CAPTURE_BYTES = 6_000_000
CAPTURE_SHA256 = "66b50c9e7b1111203dcdeee1a97dfda65997a041c64292c5e131480d00557472"


def fetch_capture(destination):
    """Download only the prefix used by the original Travis decode test."""
    with urllib.request.urlopen(CAPTURE_URL, timeout=60) as response:
        data = response.read(CAPTURE_BYTES)
    if len(data) != CAPTURE_BYTES:
        raise RuntimeError(f"Incomplete recording: {len(data)} bytes")
    destination.parent.mkdir(parents=True, exist_ok=True)
    destination.write_bytes(data)


def decode(capture):
    flowgraph = gr.top_block()
    source = blocks.file_source(gr.sizeof_gr_complex, str(capture), False)
    adapter = gsm.gsm_input(ppm=-10, osr=4, samp_rate_in=100_000_000 // 174)
    receiver = gsm.receiver(4, [0], [], False)
    demapper = gsm.gsm_bcch_ccch_demapper(timeslot_nr=0)
    decoder = gsm.control_channels_decoder()
    messages = blocks.message_debug()
    flowgraph.connect(source, adapter, receiver)
    flowgraph.msg_connect(receiver, "C0", demapper, "bursts")
    flowgraph.msg_connect(demapper, "bursts", decoder, "bursts")
    flowgraph.msg_connect(decoder, "msgs", messages, "store")
    flowgraph.run()

    decoded = []
    for index in range(messages.num_messages()):
        packet = bytes(pmt.u8vector_elements(pmt.cdr(messages.get_message(index))))
        # GSMTAP v2: header length in 32-bit words, frame number at offset 8.
        header_length = packet[1] * 4
        frame_number = struct.unpack_from("!I", packet, 8)[0]
        decoded.append(f"{frame_number}: {packet[header_length:].hex(' ')}")
    return decoded


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--capture", type=Path, required=True)
    parser.add_argument("--download", action="store_true")
    args = parser.parse_args()
    if args.download:
        fetch_capture(args.capture)
    if args.capture.stat().st_size != CAPTURE_BYTES:
        raise RuntimeError(f"Expected exactly {CAPTURE_BYTES} bytes of IQ samples")
    digest = hashlib.sha256(args.capture.read_bytes()).hexdigest()
    if digest != CAPTURE_SHA256:
        raise RuntimeError(f"Recording SHA256 mismatch: {digest}")
    print("Recording SHA256:", digest)

    expected_path = Path(__file__).with_name("bcch_expected.txt")
    expected = expected_path.read_text().splitlines()
    actual = decode(args.capture)
    # Acquisition can produce earlier messages; compare from the historical anchor.
    try:
        actual = actual[actual.index(expected[0]):]
    except ValueError:
        raise AssertionError(f"Reference frame not decoded; got {len(actual)} messages") from None
    if actual != expected:
        diff = "\n".join(difflib.unified_diff(expected, actual, fromfile="expected", tofile="decoded"))
        raise AssertionError(f"Decoded messages differ:\n{diff}")
    print(f"PASS: decoded {len(actual)} reference BCCH/CCCH messages from IQ recording")


if __name__ == "__main__":
    main()
