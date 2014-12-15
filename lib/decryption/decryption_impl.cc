/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <grgsm/endian.h>
#include <grgsm/gsmtap.h>
#include "decryption_impl.h"
#include "a5_1_2.h"

const uint32_t BURST_SIZE=148;

namespace gr {
  namespace gsm {

    decryption::sptr
    decryption::make(const std::vector<uint8_t> & k_c)
    {
      return gnuradio::get_initial_sptr
        (new decryption_impl(k_c));
    }

    /*
     * The private constructor
     */
    decryption_impl::decryption_impl(const std::vector<uint8_t> & k_c)
      : gr::block("decryption",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        set_k_c(k_c);

//        std::cout << "Be careful with decryption block - it wasn't tested yet!" << std::endl;
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&decryption_impl::decrypt, this, _1));
        message_port_register_out(pmt::mp("bursts"));
    }

    /*
     * Virtual destructor
     */
    decryption_impl::~decryption_impl()
    {
    }
    
    void decryption_impl::set_k_c(const std::vector<uint8_t> & k_c)
    {
        d_k_c = k_c;
    }
    
    void decryption_impl::decrypt(pmt::pmt_t msg)
    {
        if(d_k_c.size() != 8){
            message_port_pub(pmt::mp("bursts"), msg);
        } else 
        if(d_k_c[0] == 0 && d_k_c[1] == 0 && d_k_c[2] == 0 && d_k_c[3] == 0 &
           d_k_c[4] == 0 && d_k_c[5] == 0 && d_k_c[6] == 0 && d_k_c[7] == 0)
        {
            message_port_pub(pmt::mp("bursts"), msg);
        } else
        {
            uint8_t decrypted_data[BURST_SIZE];
            uint8_t AtoBkeystream[15];
            uint8_t BtoAkeystream[15];
            uint8_t * keystream;
            
            pmt::pmt_t header_plus_burst = pmt::cdr(msg);
            gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);
            uint8_t * burst_binary = (uint8_t *)(pmt::blob_data(header_plus_burst))+sizeof(gsmtap_hdr);
            
            uint32_t frame_number = be32toh(header->frame_number) & 0x3fff;
            bool uplink_burst = (be32toh(header->frame_number) & 0x4000) ? true : false;
            uint32_t t1 = frame_number / (26*51);
            uint32_t t2 = frame_number % 26;
            uint32_t t3 = frame_number % 51;
            uint32_t frame_number_mod = (t1 << 11) + (t3 << 5) + t2;
            keysetup(&d_k_c[0], frame_number_mod);
            runA51(AtoBkeystream, BtoAkeystream);
            
            if(uplink_burst){
                //process uplink burst
                keystream = BtoAkeystream;
            } else {
                //process downlink burst
                keystream = AtoBkeystream;
            }
            /* guard bits */
            for (int i = 0; i < 3; i++) {
                decrypted_data[i] = burst_binary[i];
            }
            //encrypt first part of the burst
            for (int i = 0; i < 57; i++) {
                decrypted_data[i+3] = keystream[i] ^ burst_binary[i+3];
            }
            /* stealing bits and midamble */
            for (int i = 60; i < 88; i++) {
                decrypted_data[i] = burst_binary[i];
            }
            //encrypt second part of the burst
            for (int i = 0; i < 57; i++) {
                decrypted_data[i+88] = keystream[i+57] ^ burst_binary[i+88];
            }
            /* guard bits */
            for (int i = 145; i < 148; i++) {
                decrypted_data[i] = burst_binary[i];
            }
            uint8_t new_header_plus_burst[sizeof(gsmtap_hdr)+BURST_SIZE];
            memcpy(new_header_plus_burst, header, sizeof(gsmtap_hdr));
            memcpy(new_header_plus_burst+sizeof(gsmtap_hdr), decrypted_data, BURST_SIZE);
            
            pmt::pmt_t msg_binary_blob = pmt::make_blob(new_header_plus_burst, sizeof(gsmtap_hdr)+BURST_SIZE);
            pmt::pmt_t msg_out = pmt::cons(pmt::PMT_NIL, msg_binary_blob);
            
            message_port_pub(pmt::mp("bursts"), msg_out);
        }
        return;
    }
  } /* namespace gsm */
} /* namespace gr */

