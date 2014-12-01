The gr-gsm project
==================
The project is based on the gsm-receiver which was written by me for the Airprobe project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

Installation
============
The project is based on GNU Radio signal processing framework and takes advantage of its great features like stream tagging and message passing.
Presence of GNU Radio is therefore a basic requirement for compilation and installation of gr-gsm. Installation of GNU Radio is described at the bottom of the page.

The description of the installation is focussed on Ubuntu distribution and its flavours.
First make sure that you have all required packages (checked on Ubuntu 14.04 and 14.10):
```
sudo apt-get install git cmake libboost-all-dev libcppunit-dev swig \
                     doxygen liblog4cpp5-dev python-scipy
```
Then download gr-gsm sources using git:
```
git clone https://github.com/ptrkrysik/gr-gsm.git
```

To compile and install gr-gsm run:
```
cd gr-gsm
mkdir build
cd build
cmake ..
make
sudo make install
```

Usage
=====
There are many possible applications of gr-gsm. At this moment there is one application that is ready out of the box. It is improved replacement of the old Airprobe - the program that lets you receive and decode GSM control messages from timeslot 0 on the broadcasting channel of a BTS. After installation of gr-gsm there are three python executables that will be installed:
-airprobe_rtlsdr.py,
-airprobe_file.py.

Airprobe with RTL-SDR input
---------------
This program uses cheap RTL-SDR receivers as the source of the signal. It can be started by running from a terminal:
```
airprogre_rtlsdr.py
```
The window of the program contains amplitude spectrum of the signal drawn in realtime. The central frequency of the signal can be changed by moving fc slider. The GSM signal has bandwidth of around 200kHz. By looking for constant hills on the spectrum of such width you can find a GSM broadcasting channel. By setting the fc slider to a carrier frequency of a broadcasting channel the program should immediately print content of subsequent messages on the standard output. If it doesn't happen set ppm slider into different positions. The slider is responsible for setting devices clock offset correction. If the clock offset is too large the clock offset correction algorithm that is implemented in the program won't work. There is intentionally added upper of allowable clock offset - it was done in order to avoid adaptation of the algorithm to neighbour channels that would inevitably lead to instability. You can use the value set later by passing it as argument of the program:
```
airprogre_rtlsdr.py -p <correction>
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
The airprobe (file,usrp,rtlsdr) application sends GSM messages in GSMTAP format that was created by Harald Welte to the UDP port number 4729. Wireshark interprets packets coming on this port as GSM data with GSMTAP header and it is able to dissect messages.
To start Wireshark straight to analysis of the GSMTAP packets obtained from gr-gsm's airprobe use following command:
```
sudo wireshark -k -Y '!icmp && gsmtap' -i lo
````
If you want to avoid the risks caused by running Wireshark with root priviledges follow this short howto:
https://ask.wireshark.org/questions/7976/wireshark-setup-linux-for-nonroot-user

Installation of GNU Radio
=========================

Building GNU Radio from source
------------------------------
For advanced users and developers it is advised to compile GNU Radio and required third party GNU Radio projects from source.

In order to compile GNU Radio with optional projects needed in order to receive data from USRP devices (uhd) and RTL-SDR devices (gr-osmosdr) run following commands:

```
sudo apt-get install git
git clone git://github.com/pybombs/pybombs
cd pybombs
./pybombs install gnuradio uhd gr-osmosdr
```

At the first run pybombs will ask for configuration options. As a target directory select /usr/local/. The rest of the options can be left as default.
Pybombs will then take care of downloading all of required libraries for installation of gnuradio and it will build and install it in /usr/local/.


Installation of GNU Radio from packages (Ubuntu 14.10)
------------------------------------------------------
The easier way to install gnuradio is installation from distribution's repository. Currently gr-gsm works with GNU Radio versions 3.7.3 and newer. On Ubuntu 14.10 it is possible to compile and use gr-gsm together with GNU Radio available from distribution's standard repository.
To do this install all required packages by entering following command in a terminal:

```
sudo apt-get install gnuradio-dev gr-osmosdr libgnuradio-uhd3.7.2.1
```

Gr-gsm blocks that will be later installed will be located in /usr/local directory. To tell Gnu Radio Companion to look for blocks in there create config.conf in ~/.gnuradio directory:
```
mkdir ~/.gnuradio
touch ~/.gnuradio/config.conf
```

then edit ~/.gnuradio/config.conf and put following text inside:
```
[grc]
local_blocks_path=/usr/local/share/gnuradio/grc/blocks:/usr/share/gnuradio/grc/blocks
```

