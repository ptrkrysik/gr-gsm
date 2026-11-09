This program processes files containing complex data - interleaved float IQ
samples (`.cfile`) - and prints the decoded BCCH/CCCH messages (the words of
the L3 messages, one per line). The messages are also sent as GSMTAP to UDP
port 4729 on localhost, to look at them in Wireshark run
`sudo wireshark -k -f udp -Y gsmtap -i lo`.

`get_bcch_msgs_from_cfile.py` is generated from the flowgraph with

    grcc get_bcch_msgs_from_cfile.grc -o .

Example of the usage:

    python3 get_bcch_msgs_from_cfile.py --samp-rate=1M --fc=940M -i input_file

where:

* `--samp-rate` - sampling frequency of the data stored in the file (default
  100e6/174 Hz),
* `--fc` - central frequency of the recorded data - it is needed for frequency
  offset correction,
* `-i` - the file containing the complex data, it has to be centred on the
  BCCH carrier of the cell (C0).

The public test recording `vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile`
(<https://github.com/ptrkrysik/test_data>, the first 6 MB of it are enough) can
be decoded with the default sampling rate:

    python3 get_bcch_msgs_from_cfile.py -i vf_call6_a725_d174_g5_Kc1EF00BAB3BAC7002.cfile
