#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Check that the public headers are installed.

Every header of include/gnuradio/gsm in the source tree has to be in the installation
prefix at the same relative path, and nothing may be installed outside of
include/gnuradio/gsm (an earlier version put the qa_utils headers to include/gsm).

    check_installed_headers.py SOURCE_DIR PREFIX
"""

from pathlib import Path
import sys


def main():
    if len(sys.argv) != 3:
        sys.exit(__doc__)
    source = Path(sys.argv[1]) / "include" / "gnuradio" / "gsm"
    prefix = Path(sys.argv[2]) / "include"
    expected = sorted(path.relative_to(source) for path in source.rglob("*.h"))
    missing = [str(path) for path in expected if not (prefix / "gnuradio" / "gsm" / path).is_file()]
    stray = sorted(str(path.relative_to(prefix)) for path in prefix.rglob("*")
                   if path.is_file() and path.parts[len(prefix.parts):][:2] != ("gnuradio", "gsm")
                   and "gsm" in path.parts)
    if missing or stray:
        sys.exit("Header installation is wrong.\n"
                 f"  not installed: {missing or 'none'}\n  outside include/gnuradio/gsm: {stray or 'none'}")
    print(f"OK: all {len(expected)} public headers are installed")


if __name__ == "__main__":
    main()
