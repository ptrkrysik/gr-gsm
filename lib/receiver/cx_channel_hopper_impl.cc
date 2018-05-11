/* -*- c++ -*- */
/* @file
 * @author (C) 2015 by Pieter Robyns <pieter.robyns@uhasselt.be>
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
#include <grgsm/gsmtap.h>
#include <grgsm/endian.h>
#include <boost/algorithm/clamp.hpp>
#include "cx_channel_hopper_impl.h"
#include "../misc_utils/freq_hopping_utils.h"

namespace gr {
  namespace gsm {

    cx_channel_hopper::sptr
    cx_channel_hopper::make(const std::vector<int> &ma, int maio, int hsn)
    {
        return gnuradio::get_initial_sptr
          (new cx_channel_hopper_impl(ma, maio, hsn));
    }

    /*
     * The private constructor
     */
    cx_channel_hopper_impl::cx_channel_hopper_impl(const std::vector<int> &ma, int maio, int hsn)
        : gr::block("cx_channel_hopper",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
        d_ma(ma),
        d_maio(maio),
        d_hsn(hsn)
    {
        d_narfcn = ma.size();

        // Check user input for GSM 05.02, p16 compliance
        if(d_narfcn < 1 || d_narfcn > 64) {
            std::cerr << "warning: clamping number of RFCNs in the MA (" << d_narfcn << "), which should be 1 <= N <= 64." << std::endl;
            d_narfcn = boost::algorithm::clamp(d_narfcn, 1, 64);
            d_ma.resize(d_narfcn);
        }

        if(d_maio < 0 || d_maio >= d_narfcn) {
            std::cerr << "warning: clamping MAIO (" << d_maio << "), which should be 0 <= MAIO < N." << std::endl;
            d_maio = boost::algorithm::clamp(d_maio, 0, d_narfcn - 1);
        }

        if(d_hsn < 0 || d_hsn > 63) {
            std::cerr << "warning: clamping HSN (" << d_hsn << "), which should be 0 <= HSN < 64." << std::endl;
            d_hsn = boost::algorithm::clamp(d_hsn, 0, 63);
        }

        message_port_register_in(pmt::mp("CX"));
        set_msg_handler(pmt::mp("CX"), boost::bind(&cx_channel_hopper_impl::assemble_bursts, this, _1));
        message_port_register_out(pmt::mp("bursts"));
    }

    /*
     * Our virtual destructor.
     */
    cx_channel_hopper_impl::~cx_channel_hopper_impl()
    {
    }

    /**
     * Given MA, MAIO, HSN, and FN, decide which frames
     * to forward to the demapper.
     */
    void cx_channel_hopper_impl::assemble_bursts(pmt::pmt_t msg)
    {
        pmt::pmt_t header_plus_burst = pmt::cdr(msg);
        gsmtap_hdr *header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);

        uint32_t frame_nr = be32toh(header->frame_number);
        uint16_t frame_ca = be16toh(header->arfcn) & 0x3FFF;    //change highest bits to '0'
                                                                //in order to leave only ARFCN number

        int mai = calculate_ma_sfh(d_maio, d_hsn, d_narfcn, frame_nr);
/*
        if(d_ma[mai] == (int)frame_ca) {
            message_port_pub(pmt::mp("bursts"), msg);
        }*/
    }

  } /* namespace gsm */
} /* namespace gr */
