The gr-gsm project
==================
The project is based on the gsm-receiver which was written by me for Airprobe project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

Installation
------------

Currently compilation of new gnuradio is required in order to run gr-gsm. 
In order to compile gnuradio on fresh Ubuntu 14.04 run following commands:

```
sudo apt-get install git
git clone git://github.com/pybombs/pybombs
cd pybombs
./pybombs install gnuradio uhd gr-osmosdr

```
At the first run pybombs will ask for configuration options. As a target directory select /usr/local/. The rest of the options can be left as default.

To download gr-gsm sources run following command:

```
git clone git@github.com:Jakotako/gr-gsm.git
```
Make sure that you have all required packages:

```
sudo apt-get install cmake libboost1.55-all-dev libcppunit-dev swig doxygen liblog4cpp5-dev
```

To compile and install gr-gsm run:

```
cd gr-gsm
mkdir build
cmake ..
make
sudo make install
```

