#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Decode the reference call of the gr-gsm test data with the installed grgsm_decode.

The recording and its burst file come from https://github.com/ptrkrysik/test_data,
see test_data.py. The checks replace the historical Travis scripts decode.sh and
decrypt.sh and add the burst file and the voice channel:

* bursts:  the BCCH messages decoded from the burst file contain the reference messages,
* decrypt: SDCCH8 with the A5/1 key decrypts to the reference messages (12 lines),
* voice:   TCH/F decoding gives the expected number of valid GSM-FR speech frames.

Usage: reference_call.py --data-dir DIR   (the data is downloaded to DIR if needed)
"""

import argparse
import difflib
from pathlib import Path
import subprocess
import sys

import test_data

FIXTURES = Path(__file__).parents[1] / "fixtures"
EXPECTED_VOICE_FRAMES = 389
GSM_FR_FRAME_BYTES = 33


def decode(*arguments, timeout=600):
    command = ["grgsm_decode", "-a", str(test_data.ARFCN), *arguments]
    result = subprocess.run(command, capture_output=True, text=True, timeout=timeout)
    if result.returncode != 0:
        raise AssertionError(f"{' '.join(command)} exited with {result.returncode}:\n{result.stderr[-2000:]}")
    return result.stdout.splitlines()


def from_anchor(lines, anchor):
    try:
        return lines[lines.index(anchor):]
    except ValueError:
        raise AssertionError(f"Reference line not decoded: {anchor}\ngot {len(lines)} lines") from None


def check_bursts(data):
    expected = (FIXTURES / "grgsm_decode_test1_expected").read_text().splitlines()
    lines = set(decode("-b", str(data["bursts"]), "-m", "BCCH", "-t", "0", "-v"))
    missing = [line for line in expected if line not in lines]
    if missing:
        raise AssertionError(f"{len(missing)} of {len(expected)} reference BCCH messages are missing, first: {missing[0]}")
    print(f"burst file: all {len(expected)} reference BCCH messages decoded")


def check_decrypt(data):
    expected = (FIXTURES / "grgsm_decode_decrypt1_expected").read_text().splitlines()
    lines = decode("-c", str(data["prefix_35m"]), "-s", str(test_data.SAMP_RATE), "-m", "SDCCH8", "-t", "1",
                   "-k", test_data.KC, "-v", "--ppm", "-10")
    lines = from_anchor(lines, expected[0])
    if lines != expected:
        diff = "\n".join(difflib.unified_diff(expected, lines, "expected", "decoded", lineterm=""))
        raise AssertionError(f"Decrypted SDCCH8 messages differ:\n{diff}")
    print(f"SDCCH8 with A5/1: {len(expected)} reference messages match")


def check_voice(data, out_dir):
    output = out_dir / "speech.au.gsm"
    output.unlink(missing_ok=True)
    decode("-c", str(data["cfile"]), "-s", str(test_data.SAMP_RATE), "-m", "TCHF", "-t", "5", "-e", "1",
           "-k", test_data.KC, "-d", "FR", "-o", str(output))
    speech = output.read_bytes()
    if len(speech) % GSM_FR_FRAME_BYTES:
        raise AssertionError(f"Speech file size {len(speech)} is not a whole number of GSM-FR frames")
    frames = [speech[i:i + GSM_FR_FRAME_BYTES] for i in range(0, len(speech), GSM_FR_FRAME_BYTES)]
    bad = [i for i, frame in enumerate(frames) if frame[0] >> 4 != 0xD]  # every GSM-FR frame starts with 0xD
    if bad:
        raise AssertionError(f"{len(bad)} frames without the GSM-FR signature, first: {bad[0]}")
    if len(frames) != EXPECTED_VOICE_FRAMES:
        raise AssertionError(f"{len(frames)} speech frames decoded, expected {EXPECTED_VOICE_FRAMES}")
    print(f"TCH/F: {len(frames)} valid GSM-FR frames ({len(frames) * 0.02:.1f} s of speech)")


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--data-dir", type=Path, required=True)
    args = parser.parse_args()
    data = test_data.prepare(args.data_dir)
    check_bursts(data)
    check_decrypt(data)
    check_voice(data, args.data_dir)
    print("PASS")


if __name__ == "__main__":
    try:
        main()
    except AssertionError as error:
        sys.exit(f"FAIL: {error}")
