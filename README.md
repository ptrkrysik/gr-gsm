The gr-gsm project
==================
The *gr-gsm* project is based on the *gsm-receiver* written by Piotr Krysik (also the main author of *gr-gsm*) for the *Airprobe* project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

Mailing list
============
Current gr-gsm project's mailing list address is following:

gr-gsm@googlegroups.com

Mailing list is a place for general discussions, questions about the usage and installation. In case of problem with installation please try to provide full information that will help reproducing it. Minimum information should contain:
- operating system with version,
- kind of installation (how gr-gsm and its dependencies were installed: with pybombs, from distibution's repository, compiled manually)
- version of gnuradio (it can be obtained with: gnuradio-companion --version)
- error messages (in case of pybombs installation they can be obtained after switching it to verbous mode with -v option).

To join the group with any e-mail addres (google account is not required) use this link:

https://groups.google.com/forum/#!forum/gr-gsm/join

Installation
============
The *gr-gsm* is based on *GNU Radio* signal processing framework and takes advantage of its great features like stream tagging and message passing.
Presence of *GNU Radio* is therefore a basic requirement for compilation and installation of *gr-gsm*. 

The easiest way to install *gr-gsm* it to use *pybombs* installer (GNU Radio install management system). Installation with this tool was tested on Ubuntu 14.04 and 14.10 (on 14.10 installation will be much faster as compilation of *GNU Radio* is not necessary).
For installation of pybombs you will need git. On Debian based distributions you can get it with:
```sh
sudo apt-get install git
```
Then download *pybombs* sources using git:
```sh
git clone https://github.com/pybombs/pybombs.git
```

Go into *pybombs* directory and configure it:
```sh
cd pybombs
./pybombs config
```

As a install prefix enter */usr/local/*. The rest of the options can be left as defaul.

On distributions that have *GNU Radio* version 3.7.3 and above in standard repository, *GNU Radio* can be installed from packages without compilation. To avoid this quite lenghty process use:
```sh
./pybombs config forcebuild ' '
```

To check *GNU Radio* version use:
```sh
apt-cache policy gnuradio-dev
```

**Then build and install *gr-gsm* with following command**:
```
sudo ./pybombs install gr-gsm
```

Pybombs will take care of downloading all of required libraries and for installation of *GNU Radio* and building *gr-gsm*.

At the end create the `~/.gnuradio/config.conf` config file so gnuradio-companion can find custom blocks of gr-gsm:

```ini
[grc]
local_blocks_path=/usr/local/share/gnuradio/grc/blocks
```

On OS X, to compile from source, you have to make sure you point cmake to the proper python version otherwise swig will crash later one at runtime
e.g. (if using gnuradio from macports):

cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/local -DPYTHON_LIBRARY=/opt/local/Library/Frameworks/Python.framework/Versions/2.7/lib -DPYTHON_INCLUDE_DIR=/opt/local/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7/ ..

Usage
=====
There are many possible applications of *gr-gsm*. At this moment there is one application that is ready out of the box. It is improved replacement of the old *Airprobe* - the program that lets you receive and decode GSM control messages from *timeslot 0* on the broadcasting channel of a BTS. After installation of *gr-gsm* there are two python executables that will be installed:

-```airprobe_rtlsdr.py```,

-```airprobe_file.py```.

Airprobe with RTL-SDR input
---------------
This program uses cheap RTL-SDR receivers as a source of the signal. It can be started by running from a terminal:
```
airprobe_rtlsdr.py
```
The window of the program contains amplitude spectrum of the signal drawn in real-time. The central frequency of the signal can be changed by moving *fc* slider. The GSM signal has bandwidth of around *200kHz*. By looking for constant hills on the spectrum of such width you can find a GSM broadcasting channel. After setting the *fc* slider to a carrier frequency of a broadcasting channel the program should immediately print content of subsequent messages on the standard output. 

If it doesn't happen, set *ppm* slider into different positions. The slider is responsible for setting devices clock offset correction. If the clock offset is too large the clock offset correction algorithm that is implemented in the program won't work. There is intentionally added upper of allowable clock offset - it was done in order to avoid adaptation of the algorithm to neighbour channels that would inevitably lead to instability. You can use the value set later by passing it as argument of the program:
```
airprobe_rtlsdr.py -p <correction>
```

Airprobe with file input
-------------
This program processes files containing complex data - interleaved float IQ samples.
Example of the usage:
```
airprobe_file.py --samp-rate=1M --fc=940M -i input_file 
```
where:
```
--samp-rate - sampling frequency of the data stored in the file,
--fc - central frequency of the recorded data - it is needed for frequency offset correction,
-i - the file containing the complex data.
```

Analyzing GSM messages in the Wireshark
-------------------------------------------
The *Airprobe* (file, rtlsdr) application sends GSM messages in *GSMTAP* format that was created by *Harald Welte* to the UDP port number 4729. Wireshark interprets packets coming on this port as GSM data with *GSMTAP* header and it is able to dissect messages.

On Debian like systems *Wireshark can be installed with:
```
sudo apt-get install wireshark
```
To start *Wireshark* straight to analysis of the *GSMTAP* packets obtained from *gr-gsm*'s airprobe use following command:
```
sudo wireshark -k -f udp -Y gsmtap -i lo
````

If you want to avoid the risks caused by running *Wireshark* with root privileges follow this short howto:
https://ask.wireshark.org/questions/7976/wireshark-setup-linux-for-nonroot-user

Manual compilation and installation of gr-gsm
=============================================

If you intend to develop gr-gsm or have problem with pyboms installation - here is description how to manually compile and install gr-gsm from source.

You will need gnuradio, rtl-sdr, gr-osmosdr and libosmocor.e For their installation look at homepages of those projects:

http://gnuradio.org/redmine/projects/gnuradio/wiki

http://sdr.osmocom.org/trac/wiki/rtl-sdr

http://sdr.osmocom.org/trac/wiki/GrOsmoSDR

http://bb.osmocom.org/trac/wiki/libosmocore

Sometimes it is possible to install these programs/libraries from distibution's repository. Keep in mind that gr-gsm need GNU Radio 3.7.3 and above.

To download gr-gsm sources run following command:

```
git clone https://github.com/ptrkrysik/gr-gsm.git
```

Make sure that you have all required packages (checked on Ubuntu 14.04 and 14.10):

```
sudo apt-get install cmake libboost-all-dev libcppunit-dev swig \
                     doxygen liblog4cpp5-dev python-scipy
```

To compile and install gr-gsm run:

```
cd gr-gsm
mkdir build
cmake ..
make
sudo make install
```

Development
===========
New features are accepted through github's pull requests. When creating pull request try to make it adress one topic (addition of a feature x, corretion of bug y).

If you wish to develop something for gr-gsm but don't know exactly what, then look for issues with label "Enhancement". Select one that you feel you are able to complete. After that claim it by commenting in the comment section of the issue. If there is any additional information about gr-gsm needed by you to make completing the task easier - just ask.

Videos
======
Short presentation of *Airprobe*'like application of *gr-gsm*:

https://www.youtube.com/watch?v=Eofnb7zr8QE

Credits
=======
*Piotr Krysik* \<ptrkrysik (at) gmail.com\> - main author and project maintainer

*Roman Khassraf* \<rkhassraf (at) gmail.com\> - blocks for demultiplexing and decoding of voice channels,  decryption block supporting all ciphers used in GSM, blocks for storing and reading GSM bursts, project planning and user support

*Pieter Robyns* \<pieter.robyns (at) uhasselt.be\> - block reversing channel hopping

*Steve* \<steve (at) segfault.net\>  - functions for decoding control channels


Thanks
======
This work is built upon the efforts made by many people to gather knowledge of GSM. 

First very significant effort of public research into GSM and its security vulnerabilities was The Hacker's Choice GSM SCANNER PROJECT. One of the results of this project was creation of a software GSM receiver by *Tvoid* - *gsm-tvoid* - which was  was the most important predecessor of *gr-gsm* and of *gsm-receiver* from the *Airprobe* project.

*Gr-gsm* wouldn't be also possible without help and inspiration by Harald Welte, Dieter Spaar and Sylvain Munaut.

Special thanks to Pawel Koszut who generously lent his USRP1 to the author of *gr-gsm* (Piotr Krysik) in 2007-2010.
