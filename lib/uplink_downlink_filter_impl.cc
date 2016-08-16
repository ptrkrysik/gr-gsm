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
#include "uplink_downlink_filter_impl.h"
#include <grgsm/gsmtap.h>
#define BURST_SIZE 148
namespace gr {
  namespace grgsm {

    uplink_downlink_filter::sptr
    uplink_downlink_filter::make()
    {
      return gnuradio::get_initial_sptr
        (new uplink_downlink_filter_impl());
    }

    /*
     * The private constructor
     */
    uplink_downlink_filter_impl::uplink_downlink_filter_impl()
      : gr::block("uplink_downlink_filter",
              gr::io_signature::make(0,0,0),
              gr::io_signature::make(0,0,0))
    {
        message_port_register_in(pmt::mp("in"));
        message_port_register_out(pmt::mp("uplink"));
        message_port_register_out(pmt::mp("downlink"));
        
        set_msg_handler(pmt::mp("in"), boost::bind(&uplink_downlink_filter_impl::process_msg, this, _1));
    }

    void uplink_downlink_filter_impl::process_msg(pmt::pmt_t msg)
    {
        gsmtap_hdr * header = (gsmtap_hdr *)(pmt::blob_data(pmt::cdr(msg)));
        bool uplink_burst = (be16toh(header->arfcn) & 0x4000) ? true : false;
        if(uplink_burst)
        {
            message_port_pub(pmt::mp("uplink"), msg);
        } else {
            message_port_pub(pmt::mp("downlink"), msg);
        }
          
    }

    /*
     * Our virtual destructor.
     */
    uplink_downlink_filter_impl::~uplink_downlink_filter_impl()
    {
    }
  } /* namespace grgsm */
} /* namespace gr */

