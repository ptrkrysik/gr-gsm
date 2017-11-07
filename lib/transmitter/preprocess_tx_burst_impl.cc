/* -*- c++ -*- */
/* @file
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
 * 
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <grgsm/gsmtap.h>
#include <grgsm/gsm_constants.h>
#include "preprocess_tx_burst_impl.h"

namespace gr {
  namespace gsm {

    preprocess_tx_burst::sptr
    preprocess_tx_burst::make()
    {
      return gnuradio::get_initial_sptr
        (new preprocess_tx_burst_impl());
    }

    /*
     * The private constructor
     */
    preprocess_tx_burst_impl::preprocess_tx_burst_impl()
      : gr::block("preprocess_tx_burst",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::intern("bursts_in"));
        message_port_register_out(pmt::intern("bursts_out"));

        set_msg_handler(pmt::intern("bursts_in"),  boost::bind(&preprocess_tx_burst_impl::process_burst,   this, _1));
    }

    /*
     * Our virtual destructor.
     */
    preprocess_tx_burst_impl::~preprocess_tx_burst_impl()
    {
    }
    
    void preprocess_tx_burst_impl::process_burst(pmt::pmt_t burst)
    {
        int8_t * header_bits = (int8_t *)(pmt::blob_data(pmt::cdr(burst)));
        int8_t * burst_bits = (int8_t *)(pmt::blob_data(pmt::cdr(burst)))+sizeof(gsmtap_hdr);

        pmt::pmt_t header_bits_pmt = pmt::make_blob(header_bits,sizeof(gsmtap_hdr));
        
        size_t burst_size = pmt::blob_length(pmt::cdr(burst))-sizeof(gsmtap_hdr);
        if(((gsmtap_hdr*)header_bits)->sub_type == GSMTAP_BURST_ACCESS){ //cut unneeded bits from the end of access bursts
          burst_size = ACCESS_BURST_SIZE; 
        } 
        pmt::pmt_t burst_bits_pmt = pmt::make_blob(burst_bits, burst_size);
        
        pmt::pmt_t pdu_first_part = pmt::car(burst);
//        pmt::pmt_t new_pdu_first_part = pmt::dict_add( pdu_first_part, pmt::intern("gsmtap_header"), header_bits_pmt );
        pmt::pmt_t new_pdu_first_part = pdu_first_part;
        
        pmt::pmt_t new_msg = pmt::cons(new_pdu_first_part, burst_bits_pmt);
        message_port_pub(pmt::intern("bursts_out"), new_msg);
    }
  } /* namespace gsm */
} /* namespace gr */

