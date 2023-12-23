The gr-gsm project 
==================
The *gr-gsm* project is based on the *gsm-receiver* written by Piotr Krysik (also the main author of *gr-gsm*) for the *Airprobe* project.

The aim is to provide set of tools for receiving information transmitted by GSM equipment/devices.

This repository has been forked from the original creator of gr-gsm to stay informed of the compatibility of the script.

Please refer to Installation and Usage Section

Installation and usage
======================



To Download and install GR-GSM on your devices. Please download and boot into DragonOS. \
⚠️ PLEASE NOTE THAT VIRTUAL MACHINES ARE NOT SUPPORTED TO RUN GR-GSM


Step 1 :

Download Dragon OS:
https://cemaxecuter.com/

Step 2 :

Flash the OS into your USB in order to boot on it.
You can do it via Balena Etcher
https://etcher.balena.io/

Step 3 :

After Flashing your USB, Boot into DragonOS, You can use it as live or persistant. I recommand you to install it on your laptop as main or dual boot.

Step 4 :

You finished installed DragonOS, Enjoy. ( Please make sure to apt-get update just in-case )



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
This repository has been forked from the original creator, GR-GSM doesn't work on recents version of linux.

\
\
\
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

*Vadim Yanitskiy* \<axilirator (at) gmail.com\> - control and data interface for the transceiver, grgsm_trx application

*Vasil Velichkov* \<vvvelichkov (at) gmail.com\> - automatic compilation of grc applications, fixes and user support

*Pieter Robyns* \<pieter.robyns (at) uhasselt.be\> - block reversing channel hopping


Thanks
======
This work is built upon the efforts made by many people to gather knowledge of GSM. 

First very significant effort of public research into GSM and its security vulnerabilities was The Hacker's Choice GSM SCANNER PROJECT. One of the results of this project was creation of a software GSM receiver by *Tvoid* - *gsm-tvoid* - which was  was the most important predecessor of *gr-gsm* and of *gsm-receiver* from the *Airprobe* project.

*Gr-gsm* wouldn't be also possible without help and inspiration by Harald Welte, Dieter Spaar and Sylvain Munaut.

Special thanks to Pawel Koszut who generously lent his USRP1 to the author of *gr-gsm* (Piotr Krysik) in 2007-2010.
