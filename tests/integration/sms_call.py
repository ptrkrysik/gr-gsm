#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Decode an SMS from a recording of the downlink and the uplink of an SDCCH/8 channel.

The data is the pair of files sms_multirtl_{downlink,uplink}_tail.cfile of
https://github.com/ptrkrysik/test_data, see test_data.py. A phone sends a short message
while two RTL-SDR dongles record the two directions. The flowgraph is the one of the
example examples/uplink_decoding_demo: two inputs, a receiver that processes the uplink,
the SDCCH/8 demapper of timeslot 1, decryption with the known key and the control
channel decoder. It checks that

* the SMS-SUBMIT sent by the phone is decoded and decrypted completely: its three LAPDm
  segments are reassembled, and the destination number and the text are the expected ones,
* the network acknowledged it: the downlink contains the RP-ACK with the same reference.

Usage: sms_call.py --data-dir DIR   (the data is downloaded to DIR if needed)
"""

import argparse
from pathlib import Path
import sys

from gnuradio import blocks, gr, gsm
import pmt

import sms_decode
import test_data

OSR = 4
# A correction of the clock of the dongles. Without it (0) one of the uplink segments of the
# message is not decoded, the values from -20 to -5 and from 2 to 20 all give the same result.
PPM = -10

EXPECTED_ADDRESS = "530316008"
EXPECTED_TEXT = "Test sms 12345678910111213141516k"
EXPECTED_TP_REFERENCE = 2
EXPECTED_RP_REFERENCE = 4


def decode(downlink, uplink):
    """Run the flowgraph, return the decoded GSMTAP packets."""
    fc = test_data.SMS_FC
    rate = test_data.SMS_SAMP_RATE
    flowgraph = gr.top_block()
    inputs = [
        gsm.gsm_input(ppm=PPM, osr=OSR, fc=fc, samp_rate_in=rate),
        gsm.gsm_input(ppm=PPM, osr=OSR, fc=fc - test_data.SMS_UPLINK_SHIFT, samp_rate_in=rate),
    ]
    receiver = gsm.receiver(OSR, [22], [2], True)  # cell allocation, TSC 2, process the uplink
    clock = gsm.clock_offset_control(fc, rate, OSR)
    demapper = gsm.gsm_sdcch8_demapper(timeslot_nr=1)
    plain = gsm.control_channels_decoder()
    decrypted = gsm.control_channels_decoder()
    decryption = gsm.decryption(test_data.SMS_KC, 1)
    messages = blocks.message_debug()

    for index, path in enumerate((downlink, uplink)):
        flowgraph.connect(blocks.file_source(gr.sizeof_gr_complex, str(path), False), inputs[index])
        flowgraph.connect((inputs[index], 0), (receiver, index))
        flowgraph.msg_connect(clock, "ctrl", inputs[index], "ctrl_in")
    flowgraph.msg_connect(receiver, "measurements", clock, "measurements")
    flowgraph.msg_connect(receiver, "C0", demapper, "bursts")
    flowgraph.msg_connect(demapper, "bursts", plain, "bursts")
    flowgraph.msg_connect(demapper, "bursts", decryption, "bursts")
    flowgraph.msg_connect(decryption, "bursts", decrypted, "bursts")
    flowgraph.msg_connect(plain, "msgs", messages, "store")
    flowgraph.msg_connect(decrypted, "msgs", messages, "store")
    flowgraph.run()
    return [bytes(pmt.u8vector_elements(pmt.cdr(messages.get_message(i)))) for i in range(messages.num_messages())]


def check(packets):
    messages = [sms_decode.gsmtap_fields(packet) for packet in packets]
    short, unfinished = sms_decode.short_messages(messages)
    if unfinished:
        raise AssertionError(f"Layer 3 messages ended in the middle of a message: {unfinished}")

    submits = [(direction, frame, parsed) for direction, frame, parsed in short
               if parsed.get("rp_type") == "RP-DATA" and parsed.get("tpdu", {}).get("type") == "SMS-SUBMIT"]
    if len(submits) != 1:
        raise AssertionError(f"Expected one SMS-SUBMIT, decoded {len(submits)} (of {len(short)} SMS messages)")
    direction, frame, parsed = submits[0]
    tpdu = parsed["tpdu"]
    problems = []
    if direction != "UL":
        problems.append(f"the SMS-SUBMIT came in the {direction} direction")
    if tpdu["address"] != EXPECTED_ADDRESS:
        problems.append(f"destination {tpdu['address']!r}, expected {EXPECTED_ADDRESS!r}")
    if tpdu["reference"] != EXPECTED_TP_REFERENCE:
        problems.append(f"TP reference {tpdu['reference']}, expected {EXPECTED_TP_REFERENCE}")
    if parsed["rp_reference"] != EXPECTED_RP_REFERENCE:
        problems.append(f"RP reference {parsed['rp_reference']}, expected {EXPECTED_RP_REFERENCE}")
    if tpdu.get("text") != EXPECTED_TEXT:
        problems.append(f"text {tpdu.get('text')!r}, expected {EXPECTED_TEXT!r}")
    if problems:
        raise AssertionError("SMS-SUBMIT differs: " + "; ".join(problems))

    acknowledgements = [(d, f, p) for d, f, p in short
                        if d == "DL" and p.get("rp_type") == "RP-ACK" and p["rp_reference"] == EXPECTED_RP_REFERENCE]
    if not acknowledgements:
        raise AssertionError(f"No RP-ACK with reference {EXPECTED_RP_REFERENCE} from the network")
    print(f"SMS-SUBMIT (frame {frame}) to {tpdu['address']}: {tpdu['text']!r}")
    print(f"acknowledged by the network with RP-ACK (frame {acknowledgements[0][1]})")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--data-dir", type=Path, required=True)
    args = parser.parse_args()
    data = test_data.prepare(args.data_dir)
    packets = decode(data["sms_downlink"], data["sms_uplink"])
    print(f"{len(packets)} messages decoded from the downlink and the uplink")
    check(packets)
    print("PASS")


if __name__ == "__main__":
    try:
        main()
    except AssertionError as error:
        sys.exit(f"FAIL: {error}")
