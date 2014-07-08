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
#include "bursts_printer_impl.h"
#include <gsmtap.h>
#include <iterator>
#include <algorithm>

namespace gr {
  namespace gsm {
    void bursts_printer_impl::bursts_print(pmt::pmt_t msg)
    {
        pmt::pmt_t burst = pmt::cdr(msg);
        int8_t * burst_elements = (int8_t *)pmt::blob_data(burst);
        size_t burst_len=pmt::blob_length(burst);

        pmt::pmt_t header_blob = pmt::car(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_blob);
        d_c0_channels.insert(header->arfcn);
        
//        d_channels_dbm.insert(header->signal_dbm);
//        std::copy(
//            d_c0_channels.begin(),
//            d_c0_channels.end(),
//            std::ostream_iterator<uint16_t>(std::cout, " ")
//        );
//        std::cout << std::endl;
//        std::copy(
//            d_channels_dbm.begin(),
//            d_channels_dbm.end(),
//            std::ostream_iterator<int>(std::cout, " ")
//        );

        for(int ii=0; ii<burst_len; ii++)
        {
          std::cout << std::setprecision(1) << static_cast<int>(burst_elements[ii]) << "";
        }
        std::cout << std::endl;
    }

    bursts_printer::sptr
    bursts_printer::make()
    {
      return gnuradio::get_initial_sptr
        (new bursts_printer_impl());
    }

    /*
     * The private constructor
     */
    bursts_printer_impl::bursts_printer_impl()
      : gr::block("bursts_printer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
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

