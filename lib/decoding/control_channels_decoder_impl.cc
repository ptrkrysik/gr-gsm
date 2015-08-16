/* -*- c++ -*- */
/*
 * @file
 * @author Piotr Krysik <ptrkrysik@gmail.com>
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
#include "control_channels_decoder_impl.h"

#define DATA_BYTES 23

namespace gr {
  namespace gsm {

    control_channels_decoder::sptr
    control_channels_decoder::make()
    {
      return gnuradio::get_initial_sptr
        (new control_channels_decoder_impl());
    }

    /*
     * Constructor
     */
    control_channels_decoder_impl::control_channels_decoder_impl()
      : gr::block("control_channels_decoder",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
              d_collected_bursts_num(0)
    {
        //initialize de/interleaver
        int j, k, B;
        for (k = 0; k < CONV_SIZE; k++)
        {
            B = k % 4;
            j = 2 * ((49 * k) % 57) + ((k % 8) / 4);
            interleave_trans[k] = B * 114 + j; //114=57 + 57
        }
        
        //initialize decoder
        FC_init(&fc_ctx, 40, 184);
        
        //setup input/output ports
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&control_channels_decoder_impl::decode, this, _1));
        message_port_register_out(pmt::mp("msgs"));
    }

    control_channels_decoder_impl::~control_channels_decoder_impl()
    {
    }

    void control_channels_decoder_impl::decode(pmt::pmt_t msg)
    {
        unsigned char iBLOCK[BLOCKS*iBLOCK_SIZE], hl, hn, conv_data[CONV_SIZE], decoded_data[PARITY_OUTPUT_SIZE];
        d_bursts[d_collected_bursts_num] = msg;
        d_collected_bursts_num++;
        //get convecutive bursts
        
        if(d_collected_bursts_num==4)
        {
            d_collected_bursts_num=0;
            //reorganize data
            for(int ii = 0; ii < 4; ii++)
            {
                pmt::pmt_t header_plus_burst = pmt::cdr(d_bursts[ii]);
                int8_t * burst_bits = (int8_t *)(pmt::blob_data(header_plus_burst))+sizeof(gsmtap_hdr);

                for(int jj = 0; jj < 57; jj++)
                {
                    iBLOCK[ii*iBLOCK_SIZE+jj] = burst_bits[jj + 3];
                    iBLOCK[ii*iBLOCK_SIZE+jj+57] = burst_bits[jj + 88]; //88 = 3+57+1+26+1
                }
            }
            //deinterleave
            for (int k = 0; k < CONV_SIZE; k++)
            {
                conv_data[k] = iBLOCK[interleave_trans[k]];
            }
            //convolutional code decode
            int errors = conv_decode(decoded_data, conv_data);
            //std::cout << "Errors:" << errors << " " << parity_check(decoded_data) << std::endl;
            // check parity
            // If parity check error detected try to fix it.

            if (parity_check(decoded_data))
            {
                FC_init(&fc_ctx, 40, 184);
                unsigned char crc_result[PARITY_OUTPUT_SIZE];
                if (FC_check_crc(&fc_ctx, decoded_data, crc_result) == 0)
                {
                    //("error: sacch: parity error (errors=%d fn=%d)\n", errors, ctx->fn);
                    //std::cout << "Uncorrectable errors!" << std::endl;
                    errors = -1;
                    return;
                } else {
                    //DEBUGF("Successfully corrected parity bits! (errors=%d fn=%d)\n", errors, ctx->fn);
                    //std::cout << "Corrected some errors" << std::endl;
                    memcpy(decoded_data, crc_result, PARITY_OUTPUT_SIZE);
                    errors = 0;
                }
            } else {
                //std::cout << "Everything correct" << std::endl;
            }
           //compress bits
           unsigned char outmsg[27];
           unsigned char sbuf_len=224;
           int i, j, c, pos=0;
           for(i = 0; i < sbuf_len; i += 8) {
                for(j = 0, c = 0; (j < 8) && (i + j < sbuf_len); j++){
                    c |= (!!decoded_data[i + j]) << j;
                }
                outmsg[pos++] = c & 0xff;
           }

           //send message with header of the first burst
            pmt::pmt_t first_header_plus_burst = pmt::cdr(d_bursts[0]);
            gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(first_header_plus_burst);
            int8_t header_plus_data[sizeof(gsmtap_hdr)+DATA_BYTES];
            memcpy(header_plus_data, header, sizeof(gsmtap_hdr));
            memcpy(header_plus_data+sizeof(gsmtap_hdr), outmsg, DATA_BYTES);
            ((gsmtap_hdr*)header_plus_data)->type = GSMTAP_TYPE_UM;
            
            pmt::pmt_t msg_binary_blob = pmt::make_blob(header_plus_data,DATA_BYTES+sizeof(gsmtap_hdr));
            pmt::pmt_t msg_out = pmt::cons(pmt::PMT_NIL, msg_binary_blob);
            
            message_port_pub(pmt::mp("msgs"), msg_out);
        }
        return;
    }
  } /* namespace gsm */
} /* namespace gr */

