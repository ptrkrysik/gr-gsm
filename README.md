The gr-gsm project 
==================
The *gr-gsm* project is based on the *gsm-receiver* written by Piotr Krysik (also the main author of *gr-gsm*) for the *Airprobe* project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

Status
======
This version of gr-gsm is ported to **GNU Radio 3.10** (pybind11, no SWIG). GNU Radio 3.9 and older and GNU Radio 3.11 are not supported. The previous SWIG based code for GNU Radio 3.7/3.8 is kept on the branch [`legacy/swig-gnuradio-3.8`](https://github.com/ptrkrysik/gr-gsm/tree/legacy/swig-gnuradio-3.8) and is no longer maintained.

| System | GNU Radio | Tested with GitHub Actions |
|---|---|---|
| Ubuntu 24.04 | 3.10.9 | build, unit tests, decoding of a recording, installed programs |
| Ubuntu 26.04 | 3.10.12 | the same |
| Ubuntu 22.04 | 3.10.1 | build, unit tests, installation |
| Debian 12 | 3.10.5 | build, unit tests, installation |
| Debian 13 | 3.10.12 | build, unit tests, installation |

Other distributions that provide GNU Radio 3.10 with its development files should work as well, but they are not tested.

Installation
============
On the systems from the table above:

```sh
sudo apt-get install --no-install-recommends \
    git build-essential cmake pkg-config \
    gnuradio gnuradio-dev gr-osmosdr libosmocore-dev \
    pybind11-dev python3-dev python3-pybind11 python3-packaging

git clone https://github.com/ptrkrysik/gr-gsm.git
cd gr-gsm
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build
sudo cmake --install build
sudo ldconfig
```

Check the installation with `python3 -c "from gnuradio import gsm; print(gsm.receiver)"` and `grgsm_livemon --help`. To remove it again run `sudo cmake --build build --target uninstall` in the same build directory.

The `libosmocore-dev` package provides `libosmocore`, `libosmocodec`, `libosmocoding` and `libosmogsm`. The Python package `scipy` is optional, it is needed only by the experimental receiver blocks (`fcch_detector`, `sch_detector`). Example flowgraphs are installed to `share/gnuradio/examples/gsm`.

Other distributions, a custom installation prefix, leftovers of an older installation and common problems are described on the wiki page [Installation](https://github.com/ptrkrysik/gr-gsm/wiki/Installation). Old installation instructions that use PyBOMBS no longer work: PyBOMBS is not maintained any more.

Programs
========
| Program | Purpose |
|---|---|
| `grgsm_livemon` | live monitor of a single GSM channel with GUI, sends the decoded bursts as GSMTAP over UDP, e.g. for Wireshark (`sudo wireshark -k -f udp -Y gsmtap -i lo`) |
| `grgsm_livemon_headless` | the same without the GUI |
| `grgsm_scanner` | scans a GSM band for base stations (`grgsm_scanner -b GSM900`, `--args` selects the device, `-l` lists devices) |
| `grgsm_capture` | records a channel to a file with the sample rate used by the decoder |
| `grgsm_channelize` | extracts channels from a wideband recording |
| `grgsm_decode` | decodes channels from a recording or a burst file, can decrypt A5/1, A5/2 and A5/3 (`-e`, `-k`) |

Example: decode the BCCH of a recording made with `grgsm_capture`:

```sh
grgsm_capture --arfcn 975 -g 40 -T 30 capture.cfile
grgsm_decode -c capture.cfile -s 1e6 -m BCCH -t 0 -v
```

Not included in this version: `grgsm_trx` and the transceiver interface for OsmocomBB.

Usage
=====
See the [wiki](https://github.com/ptrkrysik/gr-gsm/wiki): [Usage](https://github.com/ptrkrysik/gr-gsm/wiki/Usage), [Decoding How To](https://github.com/ptrkrysik/gr-gsm/wiki/Usage:-Decoding-How-To) and [RTL-SDR setup and calibration](https://github.com/ptrkrysik/gr-gsm/wiki/RTL-SDR-setup-and-calibration). Run a program with `--help` for the current options.

Questions and bug reports
=========================
Bugs and feature requests belong to the [GitHub issues](https://github.com/ptrkrysik/gr-gsm/issues). General questions about usage and installation can be asked on the mailing list [gr-gsm@googlegroups.com](https://groups.google.com/g/gr-gsm). The list is moderated: new members have to be approved and the first messages of a new member are reviewed, because of spam.

In case of a problem with installation please provide the information that helps to reproduce it:
- operating system with version,
- how gr-gsm and its dependencies were installed (from the distribution's repository, built from source),
- the version of GNU Radio (`gnuradio-config-info --version`),
- the complete error messages.

Development
===========
Build, unit tests and file-based integration tests run with GitHub Actions on Ubuntu 24.04
and Ubuntu 26.04. A build with unit tests also runs on Ubuntu 22.04 and Debian 12 and 13.
See [tests/README.md](tests/README.md) for details and local test instructions.

New features are accepted through github's pull requests. When creating pull request try to make it adress one topic (addition of a feature x, correction of bug y).

If you wish to develop something for gr-gsm but don't know exactly what, then look for issues with label "Enhancement". Select one that you feel you are able to complete. After that claim it by commenting in the comment section of the issue. If there is any additional information about gr-gsm needed by you to make completing the task easier - just ask.

Videos
======
Short presentation of *Airprobe*'like application of *gr-gsm*:

<https://www.youtube.com/watch?v=Eofnb7zr8QE>

Credits
=======
*Piotr Krysik* \<ptrkrysik (at) gmail.com\> - main author and project maintainer

*Roman Khassraf* \<rkhassraf (at) gmail.com\> - blocks for demultiplexing and decoding of voice channels,  decryption block supporting all ciphers used in GSM, blocks for storing and reading GSM bursts, project planning and user support

*Vadim Yanitskiy* \<axilirator (at) gmail.com\> - control and data interface for the transceiver, gsm_trx application

*Vasil Velichkov* \<vvvelichkov (at) gmail.com\> - automatic compilation of grc applications, fixes and user support

*Pieter Robyns* \<pieter.robyns (at) uhasselt.be\> - block reversing channel hopping


Thanks
======
This work is built upon the efforts made by many people to gather knowledge of GSM. 

First very significant effort of public research into GSM and its security vulnerabilities was The Hacker's Choice GSM SCANNER PROJECT. One of the results of this project was creation of a software GSM receiver by *Tvoid* - *gsm-tvoid* - which was  was the most important predecessor of *gr-gsm* and of *gsm-receiver* from the *Airprobe* project.

*Gr-gsm* wouldn't be also possible without help and inspiration by Harald Welte, Dieter Spaar and Sylvain Munaut.

Special thanks to Pawel Koszut who generously lent his USRP1 to the author of *gr-gsm* (Piotr Krysik) in 2007-2010.
