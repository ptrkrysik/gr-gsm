/* -*- c++ -*- */
/*
 * @file
 * @author Roman Khassraf <rkhassraf@gmail.com>
 * @section LICENSE
 *
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <grgsm/gsmtap.h>
#include "stdio.h"
#include "tch_f_decoder_impl.h"

#define DATA_BYTES 23

namespace gr {
  namespace gsm {

    tch_f_decoder::sptr
    tch_f_decoder::make(tch_mode mode, const std::string &file)
    {
      return gnuradio::get_initial_sptr
        (new tch_f_decoder_impl(mode, file));
    }

    /*
     * Constructor
     */
    tch_f_decoder_impl::tch_f_decoder_impl(tch_mode mode, const std::string &file)
      : gr::block("tch_f_decoder",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
      d_tch_mode(mode),
      d_collected_bursts_num(0)
    {
        d_speech_file = fopen( file.c_str(), "wb" );
        if (d_speech_file == NULL)
        {
            throw std::runtime_error("TCH/F Decoder: can't open file\n");
        }

        const unsigned char amr_nb_magic[6] = { 0x23, 0x21, 0x41, 0x4d, 0x52, 0x0a };

        if (d_tch_mode == MODE_SPEECH_EFR)
        {
            fwrite(amr_nb_magic, 1, 6, d_speech_file);
        }

        int j, k, B;
        for (k = 0; k < CONV_SIZE; k++)
        {
            B = k % 8;
            j = 2 * ((49 * k) % 57) + ((k % 8) / 4);
            interleave_trans[k] = B * 114 + j;
        }

        //setup input/output ports
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&tch_f_decoder_impl::decode, this, _1));
        message_port_register_out(pmt::mp("msgs"));
    }

    tch_f_decoder_impl::~tch_f_decoder_impl()
    {
    }

    void tch_f_decoder_impl::decode(pmt::pmt_t msg)
    {
        d_bursts[d_collected_bursts_num] = msg;
        d_collected_bursts_num++;

        bool stolen = false;

        if (d_collected_bursts_num == 8)
        {
            unsigned char iBLOCK[2*BLOCKS*iBLOCK_SIZE];
            SoftVector mC(CONV_SIZE);
            SoftVector mClass1_c(mC.head(378));
            SoftVector mClass2_c(mC.segment(378, 78));
            BitVector mTCHU(189);
            BitVector mTCHD(260);
            BitVector mClass1A_d(mTCHD.head(50));
            ViterbiR2O4 mVCoder;

            d_collected_bursts_num = 0;

            // reorganize data
            for (int ii = 0; ii < 8; ii++)
            {
                pmt::pmt_t header_plus_burst = pmt::cdr(d_bursts[ii]);
                int8_t * burst_bits = (int8_t *)(pmt::blob_data(header_plus_burst))+sizeof(gsmtap_hdr);

                for (int jj = 0; jj < 57; jj++)
                {
                    iBLOCK[ii*114+jj] = burst_bits[jj + 3];
                    iBLOCK[ii*114+jj+57] = burst_bits[jj + 88]; //88 = 3+57+1+26+1
                }

                if ((ii <= 3 && static_cast<int>(burst_bits[87]) == 1) || (ii >= 4 && static_cast<int>(burst_bits[60]) == 1))
                {
                    stolen = true;
                }
            }

            // deinterleave
            for (int k = 0; k < CONV_SIZE; k++)
            {
                mC[k] = iBLOCK[interleave_trans[k]];
            }

            // Decode stolen frames as FACCH/F
            if (stolen)
            {
                BitVector mU(228);
                BitVector mP(mU.segment(184,40));
                BitVector mD(mU.head(184));
                BitVector mDP(mU.head(224));
                Parity mBlockCoder(0x10004820009ULL, 40, 224);

                mC.decode(mVCoder, mU);
                mP.invert();

                unsigned syndrome = mBlockCoder.syndrome(mDP);

                if (syndrome == 0)
                {
                    unsigned char outmsg[27];
                    unsigned char sbuf_len=224;
                    int i, j, c, pos=0;
                    for(i = 0; i < sbuf_len; i += 8) {
                        for(j = 0, c = 0; (j < 8) && (i + j < sbuf_len); j++){
                            c |= (!!mU.bit(i + j)) << j;
                        }
                        outmsg[pos++] = c & 0xff;
                    }

                    pmt::pmt_t first_header_plus_burst = pmt::cdr(d_bursts[0]);
                    gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(first_header_plus_burst);
                    header->type = GSMTAP_TYPE_UM;
                    int8_t * header_content = (int8_t *)header;
                    int8_t header_plus_data[sizeof(gsmtap_hdr)+DATA_BYTES];
                    memcpy(header_plus_data, header_content, sizeof(gsmtap_hdr));
                    memcpy(header_plus_data+sizeof(gsmtap_hdr), outmsg, DATA_BYTES);

                    pmt::pmt_t msg_binary_blob = pmt::make_blob(header_plus_data,DATA_BYTES+sizeof(gsmtap_hdr));
                    pmt::pmt_t msg_out = pmt::cons(pmt::PMT_NIL, msg_binary_blob);

                    message_port_pub(pmt::mp("msgs"), msg_out);
                }
            }

            mClass1_c.decode(mVCoder, mTCHU);
            mClass2_c.sliced().copyToSegment(mTCHD, 182);

            // 3.1.2.1
            // copy class 1 bits u[] to d[]
            for (unsigned k = 0; k <= 90; k++) {
              mTCHD[2*k] = mTCHU[k];
              mTCHD[2*k+1] = mTCHU[184-k];
            }

            Parity mTCHParity(0x0b, 3, 50);

            // 3.1.2.1
            // check parity of class 1A
            unsigned sentParity = (~mTCHU.peekField(91, 3)) & 0x07;
            //unsigned calcParity = mTCHD.head(50).parity(mTCHParity) & 0x07;
            unsigned calcParity = mClass1A_d.parity(mTCHParity) & 0x07;

            // 3.1.2.2
            // Check the tail bits, too.
            unsigned tail = mTCHU.peekField(185, 4);

            bool good = (sentParity == calcParity) && (tail == 0);

            if (good)
            {
                unsigned char mTCHFrame[33];
                unsigned int  mTCHFrameLength;

                if (d_tch_mode == MODE_SPEECH_FR) // GSM-FR
                {
                    // Undo Um's importance-sorted bit ordering.
                    // See GSM 05.03 3.1 and Tablee 2.
                    VocoderFrame mVFrame;

                    BitVector payload = mVFrame.payload();
                    mTCHD.unmap(GSM::g610BitOrder, 260, payload);
                    mVFrame.pack(mTCHFrame);
                    mTCHFrameLength = 33;
                }
                else if (d_tch_mode == MODE_SPEECH_EFR) // GSM-EFR / AMR 12.2
                {
                    VocoderAMRFrame mVFrameAMR;

                    BitVector payload = mVFrameAMR.payload();
                    BitVector TCHW(260), EFRBits(244);

                    // Undo Um's EFR bit ordering.
                    mTCHD.unmap(GSM::g660BitOrder, 260, TCHW);

                    // Remove repeating bits and CRC to get raw EFR frame (244 bits)
                    for (unsigned k=0; k<71; k++)
                      EFRBits[k] = TCHW[k] & 1;

                    for (unsigned k=73; k<123; k++)
                      EFRBits[k-2] = TCHW[k] & 1;

                    for (unsigned k=125; k<178; k++)
                      EFRBits[k-4] = TCHW[k] & 1;

                    for (unsigned k=180; k<230; k++)
                      EFRBits[k-6] = TCHW[k] & 1;

                    for (unsigned k=232; k<252; k++)
                      EFRBits[k-8] = TCHW[k] & 1;

                    // Map bits as AMR 12.2k
                    EFRBits.map(GSM::g690_12_2_BitOrder, 244, payload);

                    // Put the whole frame (hdr + payload)
                    mVFrameAMR.pack(mTCHFrame);
                    mTCHFrameLength = 32;
                }
                fwrite(mTCHFrame, 1 , mTCHFrameLength, d_speech_file);
            }
        }
    }
  } /* namespace gsm */
} /* namespace gr */

