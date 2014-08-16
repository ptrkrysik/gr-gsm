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
#include "wireshark_sink_impl.h"

namespace gr {
  namespace gsm {

    wireshark_sink::sptr
    wireshark_sink::make()
    {
      return gnuradio::get_initial_sptr
        (new wireshark_sink_impl());
    }

    /*
     * The private constructor
     */
    wireshark_sink_impl::wireshark_sink_impl()
      : gr::block("wireshark_sink",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {}

    /*
     * Our virtual destructor.
     */
    wireshark_sink_impl::~wireshark_sink_impl()
    {
    }


  } /* namespace gsm */
} /* namespace gr */

