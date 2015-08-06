/* -*- c++ -*- */
/* @file
 * @author Roman Khassraf <rkhassraf@gmail.com>
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
#include "burst_sdcch_subslot_splitter_impl.h"
#include <stdio.h>
#include <grgsm/endian.h>
#include <grgsm/gsmtap.h>


namespace gr {
  namespace gsm {

    burst_sdcch_subslot_splitter::sptr
    burst_sdcch_subslot_splitter::make(splitter_mode mode)
    {
      return gnuradio::get_initial_sptr
        (new burst_sdcch_subslot_splitter_impl(mode));
    }

    /*
     * The private constructor
     */
    burst_sdcch_subslot_splitter_impl::burst_sdcch_subslot_splitter_impl(splitter_mode mode)
      : gr::block("burst_sdcch_subslot_splitter",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
      d_mode(mode)
    {     
        message_port_register_in(pmt::mp("in"));
        
        message_port_register_out(pmt::mp("out0"));
        message_port_register_out(pmt::mp("out1"));
        message_port_register_out(pmt::mp("out2"));
        message_port_register_out(pmt::mp("out3"));
        if (d_mode == SPLITTER_SDCCH8)
        {
            message_port_register_out(pmt::mp("out4"));
            message_port_register_out(pmt::mp("out5"));
            message_port_register_out(pmt::mp("out6"));
            message_port_register_out(pmt::mp("out7"));
        }
        
        set_msg_handler(pmt::mp("in"), boost::bind(&burst_sdcch_subslot_splitter_impl::process_burst, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    burst_sdcch_subslot_splitter_impl::~burst_sdcch_subslot_splitter_impl() {}

    void burst_sdcch_subslot_splitter_impl::process_burst(pmt::pmt_t msg)
    {
        pmt::pmt_t header_plus_burst = pmt::cdr(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);
        
        uint32_t frame_nr = be32toh(header->frame_number);
        uint32_t fn_mod102 = frame_nr % 102;
        
        int8_t subslot;
        
        if (d_mode == SPLITTER_SDCCH8)
        {
            subslot = d_subslots_sdcch8[fn_mod102];
        }
        else if (d_mode == SPLITTER_SDCCH4)
        {
            subslot = d_subslots_sdcch4[fn_mod102];
        }
        
        if ((subslot == -1) || (d_mode == SPLITTER_SDCCH4 && subslot > 3))
        {
            return;
        }
        
        std::string port("out");

        switch (subslot)
        {
            case 0:
                port.append("0");
                break;
            case 1:
                port.append("1");
                break;
            case 2:
                port.append("2");
                break;
            case 3:
                port.append("3");
                break;
            case 4:
                port.append("4");
                break;
            case 5:
                port.append("5");
                break;
            case 6:
                port.append("6");
                break;
            case 7:
                port.append("7");
                break;
            default:
                port.append("0");
                break;
        }

        message_port_pub(pmt::mp(port), msg);
    }
  } /* namespace gsm */
} /* namespace gr */
