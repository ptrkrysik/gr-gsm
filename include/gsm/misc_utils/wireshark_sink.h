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


#ifndef INCLUDED_GSM_WIRESHARK_SINK_H
#define INCLUDED_GSM_WIRESHARK_SINK_H

#include <gsm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace gsm {

    /*!
     * \brief <+description of block+>
     * \ingroup gsm
     *
     */
    class GSM_API wireshark_sink : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<wireshark_sink> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gsm::wireshark_sink.
       *
       * To avoid accidental use of raw pointers, gsm::wireshark_sink's
       * constructor is in a private implementation
       * class. gsm::wireshark_sink::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_WIRESHARK_SINK_H */

