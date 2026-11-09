This is a demonstration of how to perform processing of multiple GSM channels
with use of gr-gsm and GNU Radio's polyphase channelizer. It shows how to
obtain information about C0 channels with use of an RTL-SDR device.

Files:

* `wideband_processing_demo.py` - the demo script (records 3 s of samples with
  osmosdr, channelizes, decodes and prints the found C0 channels),
* `wideband_receiver.py` - hierarchical block with the channelizer and one
  `receiver_with_decoder` per channel,
* `wideband_receiver.block.yml` - GRC definition of the `wideband_receiver`
  block (to use it add this directory to `GRC_BLOCKS_PATH` and `PYTHONPATH`),
* `receiver_with_decoder.grc` - GRC hierarchical block: receiver + BCCH/CCCH
  decoder for one channel; `receiver_with_decoder.py` is generated from it
  with `grcc -o . receiver_with_decoder.grc` (this also writes a
  `receiver_with_decoder.block.yml`, which is not needed here) and is imported by
  `wideband_receiver.py`.

To test it run (from this directory):

    python3 wideband_processing_demo.py

The sample rate has to be an even multiple of 200 kHz (`-s`), the centre
frequency (`-f`) is the frequency between the two middle channels. To process
an already recorded file of samples (centred at `-f`, sampled at `-s`) instead
of recording from the RTL-SDR use `--no-record -i file.cfile`.

The directory content is related to the "Scan for C0 - block proposal"
discussion on the mailing list.
