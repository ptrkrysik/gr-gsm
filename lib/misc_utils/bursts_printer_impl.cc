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
#include <iterator>
#include <algorithm>
#include "bursts_printer_impl.h"
#include <unistd.h>

#include <iostream>

namespace gr {
  namespace gsm {
    boost::mutex printer_mutex;
    void bursts_printer_impl::bursts_print(pmt::pmt_t msg)
    {
        pmt::pmt_t header_plus_burst = pmt::cdr(msg);

        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);
        int8_t * burst = (int8_t *)(pmt::blob_data(header_plus_burst))+sizeof(gsmtap_hdr);
        size_t burst_len=pmt::blob_length(header_plus_burst)-sizeof(gsmtap_hdr);
        uint32_t frame_nr;
        
        std::cout << d_prepend_string;
        if (d_prepend_fnr) 
        {
            frame_nr = be32toh(header->frame_number);
            std::cout << frame_nr << ":";
        } 

        for(int ii=0; ii<burst_len; ii++)
        {
          std::cout << std::setprecision(1) << static_cast<int>(burst[ii]) << "";
        }
        std::cout << std::endl;
    }
     
    bursts_printer::sptr
    bursts_printer::make(pmt::pmt_t prepend_string, bool prepend_fnr)
    {
      return gnuradio::get_initial_sptr
        (new bursts_printer_impl(prepend_string, prepend_fnr));
    }

    /*
     * The private constructor
     */
    bursts_printer_impl::bursts_printer_impl(pmt::pmt_t prepend_string, bool prepend_fnr)
      : gr::block("bursts_printer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        d_prepend_string = prepend_string;
        d_prepend_fnr = prepend_fnr;
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&bursts_printer_impl::bursts_print, this, _1));
    }
    
    /*
     * Our virtual destructor.
     */
    bursts_printer_impl::~bursts_printer_impl()
    {
    }


  } /* namespace gsm */
} /* namespace gr */

