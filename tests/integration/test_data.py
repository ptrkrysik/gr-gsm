#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Fetch the reference recording of the gr-gsm test data.

The data lives in https://github.com/ptrkrysik/test_data (it used to be a git
submodule). The files are downloaded when a test needs them, from a pinned commit,
and every download is verified with its SHA-256. A file that is already in the
destination directory and matches is not downloaded again, so the directory can be
cached between runs.

    python3 test_data.py DEST_DIR

The recording is a real call (ARFCN 725, decimation 174, gain 5) made with an
SDR; the A5/1 key of the call is known, see KC.
"""

import bz2
import hashlib
from pathlib import Path
import shutil
import sys
import urllib.request

COMMIT = "e2e5b44bc2906d5f01944355ce5c83b39eec6fee"
BASE_URL = f"https://raw.githubusercontent.com/ptrkrysik/test_data/{COMMIT}/"
CALL = "vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002"

FILES = {
    "cfile": {
        "name": f"{CALL}.cfile",
        "size": 73_475_808,
        "sha256": "95785e6e94d7a11322d03958a0b1fd6fcca8a2a145c615ccbef9b6914e0c445e",
    },
    "bursts": {
        "name": f"{CALL}.bursts.bz2",
        "size": 235_616,
        "sha256": "35403366e52a82098c2b29f85016c079c91157de9ede25f7245cb5f621c9c676",
    },
    # Downlink and uplink of an SDCCH/8 channel recorded with two RTL-SDR dongles
    # while a phone sent an SMS (1 Msps, the downlink is centred on 947.8 MHz, the uplink
    # 45 MHz below it).
    "sms_downlink": {
        "name": "sms_multirtl_downlink_tail.cfile",
        "size": 32_000_000,
        "sha256": "38ccf460a95b7bdcaaf5bdfdd4a679dbfc7836108ff6110769190842db7c180e",
    },
    "sms_uplink": {
        "name": "sms_multirtl_uplink_tail.cfile",
        "size": 32_000_000,
        "sha256": "22885c78301cfe08a39c6af546d8e4953029efb1aad42f4caef4f30039eca2db",
    },
}

# Prefixes of the IQ file that the historical Travis tests used.
PREFIX_SHA256 = {
    6_000_000: "66b50c9e7b1111203dcdeee1a97dfda65997a041c64292c5e131480d00557472",
    35_800_000: "e25c3428086f50eec2c2cf07805a9a0f921768142a459c87ef802580fc31c099",
}

SAMP_RATE = 100_000_000 // 174
ARFCN = 725
KC = "0x1E,0xF0,0x0B,0xAB,0x3B,0xAC,0x70,0x02"

SMS_SAMP_RATE = 1_000_000
SMS_FC = 947.8e6
SMS_UPLINK_SHIFT = 45e6
SMS_KC = [0x46, 0x77, 0x2D, 0x54, 0xA3, 0xA3, 0xC3, 0x1C]


def sha256(path):
    digest = hashlib.sha256()
    with open(path, "rb") as handle:
        for block in iter(lambda: handle.read(1 << 20), b""):
            digest.update(block)
    return digest.hexdigest()


def _valid(path, info):
    return path.is_file() and path.stat().st_size == info["size"] and sha256(path) == info["sha256"]


def _download(url, destination, attempts=3):
    last = None
    for _ in range(attempts):
        try:
            temporary = destination.with_suffix(destination.suffix + ".part")
            with urllib.request.urlopen(url, timeout=60) as response, open(temporary, "wb") as out:
                shutil.copyfileobj(response, out, 1 << 20)
            temporary.replace(destination)
            return
        except OSError as error:  # network errors, truncated download
            last = error
    raise RuntimeError(f"Cannot download {url}: {last}")


def fetch(dest):
    """Download what is missing, verify everything, return the paths by key."""
    dest = Path(dest)
    dest.mkdir(parents=True, exist_ok=True)
    paths = {}
    for key, info in FILES.items():
        path = dest / info["name"]
        if not _valid(path, info):
            print(f"Downloading {info['name']} ({info['size']:,} bytes)", flush=True)
            _download(BASE_URL + info["name"], path)
            if not _valid(path, info):
                raise RuntimeError(f"{info['name']}: size or SHA-256 does not match")
        paths[key] = path
    return paths


def prefix(source, nbytes, destination):
    """Write the first nbytes of the IQ file, check them against the known hash."""
    destination = Path(destination)
    if not (destination.is_file() and destination.stat().st_size == nbytes):
        with open(source, "rb") as src, open(destination, "wb") as out:
            out.write(src.read(nbytes))
    if sha256(destination) != PREFIX_SHA256[nbytes]:
        raise RuntimeError(f"{destination.name}: SHA-256 of the first {nbytes} bytes does not match")
    return destination


def bursts(source, destination):
    """Decompress the burst file of the call."""
    destination = Path(destination)
    if not destination.is_file():
        with bz2.open(source, "rb") as src, open(destination, "wb") as out:
            shutil.copyfileobj(src, out)
    return destination


def prepare(dest):
    """Everything the integration tests use, in one directory."""
    dest = Path(dest)
    files = fetch(dest)
    return {
        "cfile": files["cfile"],
        "prefix_6m": prefix(files["cfile"], 6_000_000, dest / "prefix_6000000.cfile"),
        "prefix_35m": prefix(files["cfile"], 35_800_000, dest / "prefix_35800000.cfile"),
        "bursts": bursts(files["bursts"], dest / f"{CALL}.bursts"),
        "sms_downlink": files["sms_downlink"],
        "sms_uplink": files["sms_uplink"],
    }


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit(__doc__)
    for key, path in prepare(sys.argv[1]).items():
        print(f"{key}: {path}")
