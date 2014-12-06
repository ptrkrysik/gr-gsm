The gr-gsm project
==================
The *gr-gsm* project is based on the *gsm-receiver* written by Piotr Krysik (also the main author of *gr-gsm*) for the *Airprobe* project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

Installation
============
The project is based on *GNU Radio* signal processing framework and takes advantage of its great features like stream tagging and message passing.
Presence of *GNU Radio* is therefore a basic requirement for compilation and installation of *gr-gsm*. 

Installation is based on *pybombs* installer (GNU Radio install management system) and was tested on Ubuntu 14.04 and 14.10 (on 14.10 installation will be much faster as compilation of *GNU Radio* is not necessary).
For installation you will need git. On Debian based distributions you can get it with:
```
sudo apt-get install git
```
Then download *pybombs* sources using git:
```
git clone https://github.com/ptrkrysik/pybombs.git
```

Go into *pybombs* directory and configure it:
```
cd pybombs
./pybombs
```

At the first run *pybombs* will ask for configuration options. As a target directory select */usr/local/*. The rest of the options can be left as default.

Then build and install *gr-gsm* with following command:
```
./pybombs install gr-gsm
```

Pybombs will take care of downloading all of required libraries and for installation of *GNU Radio* and building *gr-gsm*.

*Gr-gsm* blocks that are installed in */usr/local* directory. To tell *Gnu Radio Companion* to look for blocks in there create *config.conf* in *~/.gnuradio* directory (this step is not required on Ubuntu 14.04 as *GNU Radio* will be built from source, will reside in */usr/local* and *GNU Radio* will be already configured to look for *Gnu Radio Companion* blocks in there):
```
mkdir ~/.gnuradio
touch ~/.gnuradio/config.conf
```

then edit ~/.gnuradio/config.conf and put following text inside:
```
[grc]
local_blocks_path=/usr/local/share/gnuradio/grc/blocks:/usr/share/gnuradio/grc/blocks
```

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
The window of the program contains amplitude spectrum of the signal drawn in real-time. The central frequency of the signal can be changed by moving *fc* slider. The GSM signal has bandwidth of around *200kHz*. By looking for constant hills on the spectrum of such width you can find a GSM broadcasting channel. By setting the *fc* slider to a carrier frequency of a broadcasting channel the program should immediately print content of subsequent messages on the standard output. 

If it doesn't happen set *ppm* slider into different positions. The slider is responsible for setting devices clock offset correction. If the clock offset is too large the clock offset correction algorithm that is implemented in the program won't work. There is intentionally added upper of allowable clock offset - it was done in order to avoid adaptation of the algorithm to neighbour channels that would inevitably lead to instability. You can use the value set later by passing it as argument of the program:
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
The *Airprobe* (file,usrp,rtlsdr) application sends GSM messages in *GSMTAP* format that was created by *Harald Welte* to the UDP port number 4729. Wireshark interprets packets coming on this port as GSM data with *GSMTAP* header and it is able to dissect messages.

On Debian like systems *Wireshark can be installed with:
```
sudo apt-get install wireshark
```
To start *Wireshark* straight to analysis of the *GSMTAP* packets obtained from *gr-gsm*'s airprobe use following command:
```
sudo wireshark -k -Y '!icmp && gsmtap' -i lo
````

If you want to avoid the risks caused by running *Wireshark* with root privileges follow this short howto:
https://ask.wireshark.org/questions/7976/wireshark-setup-linux-for-nonroot-user

Videos
======
Short presentation of *Airprobe*'like application of *gr-gsm*:
https://www.youtube.com/watch?v=Eofnb7zr8QE

Credits
=======
*Piotr Krysik* \<pkrysik (at) gmail.com\> - main author and project maintainer

*Steve* \<steve (at) segfault.net\>  - functions for decoding channel codes


Thanks
======
This work is built upon the efforts made by many people to build knowledge of GSM. 

The most important ancestor of *gr-gsm* (and *gsm-receiver* of *Airprobe* project) was *gsm-tvoid* written by Tvoid. 

*Gr-gsm* wouldn't be also possible without help and inspiration by Harald Welte, Dieter Spaar and Sylvain Munaut.

Special thanks to Pawel Koszut who lent his precious USRP1 to the author of *gr-gsm* (Piotr Krysik) in 2007-2010.
