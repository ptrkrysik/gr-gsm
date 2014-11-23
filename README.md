The gr-gsm project
==================
The project is based on the gsm-receiver which was written by me for Airprobe project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

Installation of GNU Radio
=========================

The project is based on GNU Radio signal processing framework and takes advantage of its great features like stream tagging and message passing.
Installation of GNU Radio is therefore a basic requirement for compilation and installation of gr-gsm.


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
Pybombs will then take care of downloading all of required libraries for installation of gnuradio and build it will build and install it in /usr/local/.


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

Compilation and installation of gr-gsm
======================================

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


