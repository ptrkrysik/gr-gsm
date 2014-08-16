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

#ifndef INCLUDED_GSM_WIRESHARK_SINK_IMPL_H
#define INCLUDED_GSM_WIRESHARK_SINK_IMPL_H

#include <gsm/misc_utils/wireshark_sink.h>

namespace gr {
  namespace gsm {

    class wireshark_sink_impl : public wireshark_sink
    {
     private:
      // Nothing to declare in this block.

     public:
      wireshark_sink_impl();
      ~wireshark_sink_impl();

    }; // namespace gsm
  }
} // namespace gr

#endif /* INCLUDED_GSM_WIRESHARK_SINK_IMPL_H */

