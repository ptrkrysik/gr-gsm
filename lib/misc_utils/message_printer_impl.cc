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
#include "message_printer_impl.h"
#include "gsm/gsmtap.h"

namespace gr {
  namespace gsm {

    void message_printer_impl::message_print(pmt::pmt_t msg)
    {
        pmt::pmt_t message = pmt::cdr(msg);
        uint8_t * message_elements = (uint8_t *)pmt::blob_data(message);
        size_t message_len=pmt::blob_length(message);

//        pmt::pmt_t header_blob = pmt::car(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)message_elements;
        
        for(int ii=(header->hdr_len*4); ii<message_len; ii++)
        {
            printf(" %02x", message_elements[ii]);
        }
        std::cout << std::endl;
    }

    message_printer::sptr
    message_printer::make()
    {
      return gnuradio::get_initial_sptr
        (new message_printer_impl());
    }

    /*
     * The private constructor
     */
    message_printer_impl::message_printer_impl()
      : gr::block("message_printer",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::mp("msgs"));
        set_msg_handler(pmt::mp("msgs"), boost::bind(&message_printer_impl::message_print, this, _1));

    }

    /*
     * Our virtual destructor.
     */
    message_printer_impl::~message_printer_impl()
    {
    }
  } /* namespace gsm */
} /* namespace gr */

