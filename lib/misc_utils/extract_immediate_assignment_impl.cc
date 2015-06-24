/* -*- c++ -*- */
/*
 * @file
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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <grgsm/gsmtap.h>
#include <unistd.h>
#include <set>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "extract_immediate_assignment_impl.h"

namespace gr {
  namespace gsm {
    boost::mutex extract_immediate_assignment_mutex;

    void extract_immediate_assignment_impl::process_message(pmt::pmt_t msg){
        pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
        uint8_t * message_plus_header = (uint8_t *)pmt::blob_data(message_plus_header_blob);
        gsmtap_hdr * header = (gsmtap_hdr *)message_plus_header;
        uint8_t * msg_elements = (uint8_t *)(message_plus_header+sizeof(gsmtap_hdr));
        uint32_t frame_nr = be32toh(header->frame_number);

        if(msg_elements[2]==0x3f)
        {
            std::cout << "\n------------------------------------------------------------------\n" << std::endl;
            std::cout << "Immediate assignment found !" << std::endl;
            std::cout << "FrameNr: " << (unsigned)frame_nr << std::endl;

            uint8_t mode = msg_elements[3] & (1 << 4);

            /*
                channel description, see table 10.23 in GSM 04.08

                msg_elements[4], octet 2 in specs

                5 bits channel type
                    ignored in TBF
                    00001   TCH/F
                    0001T   TCH/H, subchannel/TDMA offset T
                    001TT   SDCCH/4, subchannel/TDMA offset TT
                    01TTT   SDCCH/8, subchannel/TDMA offset TTT
                3 bits timeslot number TN
            */
            uint8_t channeltype = (msg_elements[4] >> 3);
            uint8_t timeslot = (msg_elements[4] & 7);
            uint8_t subchannel;
            std::string channel_type;

            std::cout << "Timeslot: " << (unsigned)timeslot << std::endl;

            if (mode == 0)
            {
                if (channeltype >= 8)
                {
                    channel_type = "SDCCH/8";
                    subchannel = (channeltype & 7);
                }
                else if (channeltype >= 4 && channeltype <= 7)
                {
                    channel_type = "SDCCH/4";
                    subchannel = (channeltype & 3);
                }
                else if (channeltype >= 2 && channeltype <= 3)
                {
                    channel_type = "TCH/H";
                    subchannel = (channeltype & 1);
                }
                else
                {
                    channel_type = "TCH/F";
                    subchannel = 0;
                }

                std::cout << "Channel type: " << channel_type << std::endl;
                std::cout << "Subchannel: " << (unsigned)subchannel << std::endl;
            }
            else
            {
                std::cout << "Channel type: " << "GPRS - Temporary Block Flow TBF" << std::endl;
            }

            /*
                msg_elements[5], msg_elements[6] are octets 3 and 4 in specs

                    3 bits training sequence (we dont process this for the moment)
                    1 bit hopping channel H

                    if H = 0:
                        2 bit spare
                        2 bit high part of single channel arfcn

                        8 bit low part of single channel arfcn

                    if H = 1:
                        4 bit high part of MAIO

                        2 bit low part of MAIO
                        6bit HSN
            */
            uint8_t hopping = (msg_elements[5] >> 4) & 1;

            std::cout << "Hopping: " << (unsigned)hopping << std::endl;

            if (hopping)
            {
                uint8_t maio = (msg_elements[5] & 0xf) << 2;
                maio |= (msg_elements[6] >> 6);

                uint8_t hsn = (msg_elements[6] & 0x3f);

                std::cout << "MAIO: " << (unsigned)maio << std::endl;
                std::cout << "HSN: " << (unsigned)hsn << std::endl;
            }
            else
            {
                uint16_t arfcn = (msg_elements[5] & 3) << 8;
                arfcn |= msg_elements[6];

                std::cout << "ARFCN: " << (unsigned)arfcn << std::endl;
            }

            // request reference as ID in set, so we get only one immediate assignment per reference
            /*
                msg_elements[7 - 9], octets 5 - 7 in specs : request reference, maybe later

                msg_elements[10]:   timing advance
            */
            uint8_t timing_advance = msg_elements[10];
            std::cout << "TA: " << (unsigned)timing_advance << std::endl;

            /*
                msg_elements[11] - 20:   mobile allocation, flexible length, see 10.5.2.21
            */

            uint8_t mobile_allocation_len = msg_elements[11];
            if (mobile_allocation_len > 0)
            {
                uint8_t mobile_allocation[mobile_allocation_len];
                for (int i=0; i<mobile_allocation_len; i++)
                {
                    mobile_allocation[i] = msg_elements[12 + i];
                    std::cout << "MA: " << (unsigned)mobile_allocation[i] << std::endl;
                }
            }
        }
    }

    extract_immediate_assignment::sptr
    extract_immediate_assignment::make()
    {
      return gnuradio::get_initial_sptr
        (new extract_immediate_assignment_impl());
    }

    /*
     * The private constructor
     */
    extract_immediate_assignment_impl::extract_immediate_assignment_impl()
      : gr::block("extract_immediate_assignment",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::mp("msgs"));
        set_msg_handler(pmt::mp("msgs"), boost::bind(&extract_immediate_assignment_impl::process_message, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    extract_immediate_assignment_impl::~extract_immediate_assignment_impl()
    {
    }
  } /* namespace gsm */
} /* namespace gr */

