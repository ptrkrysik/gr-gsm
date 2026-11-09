#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Run the installed command line tools on the historical Travis IQ recording.

* every installed program starts and prints its help,
* grgsm_decode reproduces the reference BCCH output,
* grgsm_livemon_headless, fed from the file, sends GSMTAP packets over UDP.

The recording is the one used by decode_capture.py (run that script with
--download first, or pass an existing --capture).
"""

import argparse
import difflib
from pathlib import Path
import shutil
import socket
import subprocess
import threading
import time

PROGRAMS = [
    "grgsm_decode", "grgsm_scanner", "grgsm_capture", "grgsm_channelize",
    "grgsm_livemon", "grgsm_livemon_headless",
]
SAMP_RATE = 100_000_000 // 174
ANCHOR = "860933 1329237:  59 06 1a 8f 6d 18 10 80 00 00 00 00 00 00 00 00 00 00 00 78 b9 00 00"


def check_help():
    for program in PROGRAMS:
        path = shutil.which(program)
        if path is None:
            raise AssertionError(f"{program} is not installed")
        result = subprocess.run([path, "--help"], capture_output=True, text=True, timeout=120)
        if result.returncode != 0 or "usage" not in (result.stdout + result.stderr).lower():
            raise AssertionError(f"{program} --help failed:\n{result.stdout}\n{result.stderr}")
        print(f"{program}: --help OK")


def check_decode(capture, expected_path):
    command = ["grgsm_decode", "-c", str(capture), "-s", str(SAMP_RATE), "-m", "BCCH",
               "-t", "0", "-v", "--ppm", "-10"]
    result = subprocess.run(command, capture_output=True, text=True, timeout=300)
    lines = result.stdout.splitlines()
    try:
        lines = lines[lines.index(ANCHOR):]
    except ValueError:
        raise AssertionError(f"Reference line not decoded, exit {result.returncode}:\n{result.stderr}") from None
    expected = expected_path.read_text().splitlines()
    if lines != expected:
        diff = "\n".join(difflib.unified_diff(expected, lines, "expected", "decoded", lineterm=""))
        raise AssertionError(f"grgsm_decode output differs:\n{diff}")
    print(f"grgsm_decode: {len(lines)} reference lines match")


def check_livemon(capture, seconds, min_packets):
    count = 0
    stop = threading.Event()
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.bind(("127.0.0.1", 4729))
    sock.settimeout(0.5)

    def receive():
        nonlocal count
        while not stop.is_set():
            try:
                data, _ = sock.recvfrom(4096)
            except socket.timeout:
                continue
            if len(data) > 16 and data[0] == 2:  # GSMTAP version 2
                count += 1

    thread = threading.Thread(target=receive)
    thread.start()
    process = subprocess.Popen(
        ["grgsm_livemon_headless", "--args", f"file={capture},rate={SAMP_RATE},throttle=true",
         "-s", str(SAMP_RATE), "-f", "936.0M", "-o", "0", "-p", "-10", "--serverport", "4800"],
        stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        time.sleep(seconds)
        if process.poll() is not None and count == 0:
            raise AssertionError(f"grgsm_livemon_headless exited with {process.returncode}")
    finally:
        process.terminate()
        try:
            process.wait(timeout=20)
        except subprocess.TimeoutExpired:
            process.kill()
        stop.set()
        thread.join()
        sock.close()
    if count < min_packets:
        raise AssertionError(f"Only {count} GSMTAP packets received, expected at least {min_packets}")
    print(f"grgsm_livemon_headless: {count} GSMTAP packets received")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--capture", type=Path, required=True)
    parser.add_argument("--livemon-seconds", type=int, default=30)
    args = parser.parse_args()
    check_help()
    check_decode(args.capture, Path(__file__).parents[1] / "fixtures" / "grgsm_decode_test1_expected")
    check_livemon(args.capture, args.livemon_seconds, 50)
    print("PASS")


if __name__ == "__main__":
    main()
