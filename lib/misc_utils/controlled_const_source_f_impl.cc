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
#include "controlled_const_source_f_impl.h"

namespace gr {
  namespace gsm {

    controlled_const_source_f::sptr
    controlled_const_source_f::make(float constant)
    {
      return gnuradio::get_initial_sptr
        (new controlled_const_source_f_impl(constant));
    }
    
    void controlled_const_source_f_impl::set_constant_msg(pmt::pmt_t msg){
      if(pmt::is_real(msg)){
          set_constant(pmt::to_double(msg));
      }
    }

    /*
     * The private constructor
     */
    controlled_const_source_f_impl::controlled_const_source_f_impl(float constant)
      : gr::sync_block("controlled_const_source_f",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(float)))
    {
        set_constant(constant);
        message_port_register_in(pmt::mp("constant_msg"));
        set_msg_handler(pmt::mp("constant_msg"), boost::bind(&controlled_const_source_f_impl::set_constant_msg, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    controlled_const_source_f_impl::~controlled_const_source_f_impl()
    {
    }

    int
    controlled_const_source_f_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
      float *optr = (float*)output_items[0];
      float t;
      
      t = d_constant;
      std::fill_n(optr, noutput_items, t);

      return noutput_items;
    }
    
    void controlled_const_source_f_impl::set_constant(float constant){
      d_constant = constant;
    }

  } /* namespace gsm */
} /* namespace gr */

