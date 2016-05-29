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
//#include <gnuradio/msg_queue.h>
#include "msg_to_tag_impl.h"

namespace gr {
  namespace grgsm {

    msg_to_tag::sptr
    msg_to_tag::make()
    {
      return gnuradio::get_initial_sptr
        (new msg_to_tag_impl());
    }

//    void msg_to_tag_impl::queue_msg(pmt::pmt_t msg){
//        
//    }

    /*
     * The private constructor
     */
    msg_to_tag_impl::msg_to_tag_impl()
      : gr::sync_block("msg_to_tag",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
        message_port_register_in(pmt::mp("msg"));
//        set_msg_handler(pmt::mp("msg"), boost::bind(&msg_to_tag::queue_msg, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    msg_to_tag_impl::~msg_to_tag_impl()
    {
    }

    int
    msg_to_tag_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      memcpy(out, in, sizeof(gr_complex)*noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace grgsm */
} /* namespace gr */

