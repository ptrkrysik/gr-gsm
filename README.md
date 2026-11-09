The gr-gsm project 
==================
The *gr-gsm* project is based on the *gsm-receiver* written by Piotr Krysik (also the main author of *gr-gsm*) for the *Airprobe* project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

Status
======
This version of gr-gsm is ported to **GNU Radio 3.10** (pybind11, no SWIG). It is built and tested with GitHub Actions on the GNU Radio packages of Ubuntu 24.04 (3.10.9) and Ubuntu 26.04 (3.10.12). GNU Radio 3.11 is not supported yet. The older GNU Radio 3.7 and 3.8 are not supported by this branch; the last version for them is the `maint-3.8` branch of the original repository.

Installation instructions in the wiki and in many web pages describe the old, SWIG based versions and do not apply to this version, please use the instructions below.

Installation
============
On Ubuntu 24.04 or newer, and on Debian based distributions with GNU Radio 3.10 in general:

```sh
sudo apt-get install --no-install-recommends \
    git build-essential cmake pkg-config \
    gnuradio gnuradio-dev gr-osmosdr libosmocore-dev \
    pybind11-dev python3-dev python3-pybind11 python3-packaging

git clone https://github.com/ptrkrysik/gr-gsm.git
cd gr-gsm
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
sudo cmake --install build
sudo ldconfig
```

Check the installation with `python3 -c "from gnuradio import gsm; print(gsm.receiver)"` and `grgsm_livemon --help`.

If the Python module is not found, the installation prefix is not in the Python search path. Install to `/usr/local` (the default) or set `PYTHONPATH` to the `dist-packages` directory under your prefix and `LD_LIBRARY_PATH` to its `lib` directory.

The libraries `libosmocoding`, `libosmocodec`, `libosmogsm` and `libosmocore` come with the `libosmocore-dev` package. Other distributions need the equivalent packages. The unit tests are run with `ctest --test-dir build`.

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
General background on usage can still be found in the project's [wiki](https://osmocom.org/projects/gr-gsm/wiki/index), but commands, options and installation steps from the wiki may differ from this version. Run a program with `--help` for the current options.

Mailing list
============
Current gr-gsm project's mailing list address is following:

gr-gsm@googlegroups.com

Mailing list is a place for general discussions, questions about the usage and installation. In case of problem with installation please try to provide full information that will help reproducing it. Minimum information should contain:
- operating system with version,
- kind of installation (how gr-gsm and its dependencies were installed: with pybombs, from distibution's repository, compiled manually)
- version of gnuradio (it can be obtained with: gnuradio-companion --version)
- error messages (in case of pybombs installation they can be obtained after switching it to verbous mode with -v option).

To join the group with any e-mail address, use this link:

<https://groups.google.com/forum/#!forum/gr-gsm/join>

Development
===========
Build and file-based integration tests run with GitHub Actions on Ubuntu 24.04
and Ubuntu 26.04. See [tests/README.md](tests/README.md) for details and local
test instructions.

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
