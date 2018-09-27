/* -*- c++ -*- */
/* @file
 * @author (C) 2015 by Piotr Krysik <ptrkrysik@gmail.com>
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
#include "tmsi_dumper_impl.h"
#include "grgsm/gsmtap.h"
#include <iostream>
#include <boost/format.hpp>

namespace gr {
  namespace gsm {

    inline std::string tmsi_dumper_impl::write_timestamp(tm * now)
    {
        return boost::str(boost::format("%d%02d%02d%02d%02d%02d")
                  % (now->tm_year + 1900-2000)                   //year -2000 here after the 1900 leaves you with 15 instead of 2015 (delivery reports format is 150112223501)
                  % (now->tm_mon + 1)                           //month
                  % now->tm_mday                                //day
                  % now->tm_hour % now->tm_min % now->tm_sec);  //time of day
    }

    inline int swap(uint8_t c)
    {
        uint8_t temp1, temp2;
        temp1 = c & 0x0F;
        temp2 = c & 0xF0;
        temp1=temp1 << 4;
        temp2=temp2 >> 4;
        return(temp2|temp1);
    }

    std::string tmsi_dumper_impl::write_imsi(uint8_t * imsi)
    {
        return boost::str(boost::format("%1x%02x%02x%02x%02x%02x%02x%02x")
                  % (swap(imsi[0]) & 0x0f)
                  % swap(imsi[1]) % swap(imsi[2]) % swap(imsi[3]) % swap(imsi[4]) % swap(imsi[5]) % swap(imsi[6]) % swap(imsi[7]));
    }

    std::string tmsi_dumper_impl::write_tmsi(uint8_t * tmsi)
    {
        return boost::str(boost::format("%02x%02x%02x%02x")
                  % (int)tmsi[0] % (int)tmsi[1] % (int)tmsi[2] % (int)tmsi[3]);
    }

    void tmsi_dumper_impl::tmsi_dump_paging_request_1(uint8_t * msg_elements)
    {
      time_t t = time(0);
      tm *now = localtime(&t);
      std::string text = "";
      uint8_t msg_len = msg_elements[0];
      uint8_t mobile_identity_type = msg_elements[5] & 0x07;
      unsigned int next_element_index = 0;
      bool found_id_element = false;

      if(mobile_identity_type == 0x04) //identity type: TMSI
      {
          text = text + write_tmsi(msg_elements+6);
          text = text + "-";
          text = text + write_timestamp(now);
          text = text + "-0";
          tmsi_print(text);
          text = "";

          next_element_index = 10;
          found_id_element = true;
      } else if(mobile_identity_type == 0x01) //identity type: IMSI
      {
          text = text + "0-";
          text = text + write_timestamp(now);
          text = text + "-";
          text = text + write_imsi(msg_elements+5);
          tmsi_print(text);
          text = "";

          next_element_index = 13;
          found_id_element = true;
      }

      if(found_id_element == true)
      {
          uint8_t element_id = msg_elements[next_element_index];
          if((next_element_index < (msg_len+1)) && (element_id == 0x17)) {

              uint8_t element_len = msg_elements[next_element_index+1];
              mobile_identity_type = msg_elements[next_element_index+2] & 0x07;

              if(mobile_identity_type == 0x04) //identity type: TMSI
              {
                  text = text + write_tmsi(msg_elements+next_element_index+3); //write starting from position of the TMSI in the message
                  text = text + "-";
                  text = text + write_timestamp(now);
                  text = text + "-0";
                  tmsi_print(text);
              } else if(mobile_identity_type == 0x01) //identity type: IMSI
              {
                  text = text + "0-";
                  text = text + write_timestamp(now);
                  text = text + "-";
                  text = text + write_imsi(msg_elements+next_element_index+2); //write starting from position of the IMSI in the message
                  tmsi_print(text);
              }
          }
      }
    }

    void tmsi_dumper_impl::tmsi_dump_paging_request_2(uint8_t * msg_elements)
    {
      time_t t = time(0);
      tm *now = localtime(&t);
      std::string text = "";

      uint8_t msg_len = msg_elements[0];
      uint8_t mobile_identity_type = msg_elements[14] & 0x07;

      text = text + write_tmsi(msg_elements+4);//1st tmsi location
      text = text + "-";
      text = text + write_timestamp(now);
      text = text + "-0";
      tmsi_print(text);
      text = "";

      text = text + write_tmsi(msg_elements+8);//2nd tmsi location
      text = text + "-";
      text = text + write_timestamp(now);
      text = text + "-0";
      tmsi_print(text);
      text = "";

      if(mobile_identity_type == 0x04) //identity type: TMSI
      {
          text = text + write_tmsi(msg_elements+15);
          text = text + "-";
          text = text + write_timestamp(now);
          text = text + "-0";
          tmsi_print(text);
      } else if(mobile_identity_type == 0x01) //identity type: IMSI
      {
          text = text + "0-";
          text = text + write_timestamp(now);
          text = text + "-";
          text = text + write_imsi(msg_elements+14);
          tmsi_print(text);
      }
    }

    void tmsi_dumper_impl::tmsi_dump_paging_request_3(uint8_t * msg_elements)
    {
      time_t t = time(0);
      tm *now = localtime(&t);
      std::string text = "";

      int TMSI_INDEX[4] = {4,8,12,16}; // indexes of the 4 tmsi's

      for(int x =0; x < 4; x++)
      {
          text = text + write_tmsi(msg_elements+TMSI_INDEX[x]);
          text = text + "-";
          text = text + write_timestamp(now);
          text = text + "-0";
          tmsi_print(text);
          text = "";
      }
    }

    void tmsi_dumper_impl::tmsi_match_paging_request(pmt::pmt_t msg)
    {

      pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
      uint8_t * message_plus_header = (uint8_t *)pmt::blob_data(message_plus_header_blob);
      gsmtap_hdr * header = (gsmtap_hdr *)message_plus_header;
      uint8_t * msg_elements = (uint8_t *)(message_plus_header+sizeof(gsmtap_hdr));

      uint8_t direction_and_protocol = msg_elements[1];
      uint8_t msg_type = msg_elements[2];

      if( direction_and_protocol == 0x06 &&                    //direction from originating site, transaction id==0, Radio Resouce Management protocol
          (msg_type==0x21 || msg_type==0x22 || msg_type==0x24) //types corresponding to paging requests
          )
          {
            //write timestamp
            switch(msg_type) {
              case 0x21: //Paging Request Type 1
              {
                tmsi_dump_paging_request_1(msg_elements);
              }
              break;
              case 0x22: //Paging Request Type 2
              {
                tmsi_dump_paging_request_2(msg_elements);
              }
              break;
              case 0x24: //Paging Request Type 3
              {
                tmsi_dump_paging_request_3(msg_elements);
              }
              break;
            }
          }
    }

    void tmsi_dumper_impl::tmsi_print(std::string text)
    {

      if (d_print_message)
      {
          std::ostringstream out;
          //out << d_prepend_string;
          out << text ;
          out << std::endl;
          std::cout << out.str() << std::flush;
      }

      text = text + "\n";
      const char *serialized = text.data();
      d_output_file.write(serialized, text.length());
    }


    tmsi_dumper::sptr
    tmsi_dumper::make(pmt::pmt_t prepend_string, const std::string &filename, bool print_message)
    {
        return gnuradio::get_initial_sptr
               (new tmsi_dumper_impl(prepend_string, filename, print_message));
    }

    /*
     * The private constructor
     */
    tmsi_dumper_impl::tmsi_dumper_impl(pmt::pmt_t prepend_string, const std::string &filename, bool print_message)
        : gr::block("tmsi_dumper",
                    gr::io_signature::make(0, 0, 0),
                    gr::io_signature::make(0, 0, 0)),
                    d_output_file(filename.c_str(), std::ofstream::binary)
    {
        d_prepend_string = prepend_string;
        d_print_message = print_message;

        message_port_register_in(pmt::mp("in"));
        set_msg_handler(pmt::mp("in"), boost::bind(&tmsi_dumper_impl::process_message, this, _1));
    }

    /*
     * Our virtual destructor.
     */
    tmsi_dumper_impl::~tmsi_dumper_impl()
    {
        if (d_output_file.is_open())
        {
            d_output_file.close();
        }
    }

    void tmsi_dumper_impl::process_message(pmt::pmt_t msg)
    {
        tmsi_match_paging_request(msg);
    }
  } /* namespace gsm */
} /* namespace gr */
