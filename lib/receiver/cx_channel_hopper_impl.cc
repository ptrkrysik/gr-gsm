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
#include "cx_channel_hopper_impl.h"

namespace gr {
  namespace gsm {

    cx_channel_hopper::sptr
    cx_channel_hopper::make(int ma, int maio, int hsn)
    {
      return gnuradio::get_initial_sptr
        (new cx_channel_hopper_impl(ma, maio, hsn));
    }

    /*
     * The private constructor
     */
    cx_channel_hopper_impl::cx_channel_hopper_impl(int ma, int maio, int hsn)
      : gr::block("cx_channel_hopper",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
      d_ma = ma;
      d_maio = maio;
      d_hsn = hsn;

      message_port_register_in(pmt::mp("CX"));
      message_port_register_out(pmt::mp("bursts"));
    }

    /*
     * Our virtual destructor.
     */
    cx_channel_hopper_impl::~cx_channel_hopper_impl()
    {
    }

  } /* namespace gsm */
} /* namespace gr */

