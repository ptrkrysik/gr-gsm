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
#include "tmsi_dumper_impl.h"
#include "grgsm/gsmtap.h"
#include <stdio.h>

namespace gr {
  namespace gsm {
   
    void tmsi_dumper_impl::dump_tmsi(pmt::pmt_t msg)
    {
        pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
        uint8_t * message_plus_header = (uint8_t *)pmt::blob_data(message_plus_header_blob);
        size_t message_plus_header_len=pmt::blob_length(message_plus_header_blob);
        
        gsmtap_hdr * header = (gsmtap_hdr *)message_plus_header;
        
        for(int ii=sizeof(gsmtap_hdr); ii<message_plus_header_len; ii++)
        {
            printf(" %02x", message_plus_header[ii]);
        }
        std::cout << std::endl;
    }
    
    tmsi_dumper::sptr
    tmsi_dumper::make()
    {
      return gnuradio::get_initial_sptr
        (new tmsi_dumper_impl());
    }

    /*
     * The private constructor
     */
    tmsi_dumper_impl::tmsi_dumper_impl()
      : gr::block("tmsi_dumper",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::mp("msgs"));
        set_msg_handler(pmt::mp("msgs"), boost::bind(&tmsi_dumper_impl::dump_tmsi, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    tmsi_dumper_impl::~tmsi_dumper_impl()
    {
    }
  } /* namespace gsm */
} /* namespace gr */

