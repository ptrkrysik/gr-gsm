
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
#include <grgsm/gsmtap.h>

#include "freq_hopping_impl.h"
#include "../misc_utils/freq_hopping_utils.h"
extern "C" {
#include <osmocom/gsm/gsm_utils.h>
}

namespace gr {
  namespace gsm {
    using namespace pmt;

    freq_hopping::sptr
    freq_hopping::make(
      pmt_t hopping_cmd)
    {
      return gnuradio::get_initial_sptr
        (new freq_hopping_impl(hopping_cmd));
    }

    /*
     * The private constructor
     */
    freq_hopping_impl::freq_hopping_impl(
      pmt_t hopping_cmd
    ) : gr::block("freq_hopping",
          gr::io_signature::make(0, 0, 0),
          gr::io_signature::make(0, 0, 0)),
          d_hopping_cmd(PMT_NIL),
          d_hopping_enable(false),
          d_base_freq(890e6)
    {
        // Register I/O ports
        message_port_register_in(mp("hopping_cmd"));
        message_port_register_in(mp("bursts_in"));
        message_port_register_out(mp("bursts_out"));

        // Bind message handlers
        set_msg_handler(mp("hopping_cmd"),
          boost::bind(&freq_hopping_impl::add_hopping_cmd,
            this, _1));
        
        set_msg_handler(mp("bursts_in"),
          boost::bind(&freq_hopping_impl::set_freq_metadata,
            this, _1));
        
        add_hopping_cmd(hopping_cmd);
    }

    /*
     * Our virtual destructor.
     */
    freq_hopping_impl::~freq_hopping_impl()
    {
    }
    
    void freq_hopping_impl::add_hopping_cmd(pmt_t cmd) //TODO: fn and discard not supported at the moment
    {
      if(dict_ref(cmd,intern("cmd"), PMT_NIL) == intern("start")) {
        if(dict_ref(cmd,intern("fn"), PMT_NIL)  != PMT_NIL){
          //TODO add the command to the map<int,pmt_t>
        } else {
          pmt_t hopping_params = dict_ref(cmd, intern("hopping_params"), PMT_NIL);
          d_hopping_enable = set_hopping_params(hopping_params);
        } 
      } else if(dict_ref(cmd,intern("cmd"), PMT_NIL) == intern("stop")) {
        if(dict_ref(cmd,intern("fn"),PMT_NIL)  != PMT_NIL){
          //TODO add the command to the map<int,pmt_t>
          
        } else {
          d_hopping_enable = false;
        }
      }
    }
    
    void freq_hopping_impl::set_freq_metadata(pmt_t burst)
    {

      if(d_hopping_enable) {
        pmt_t pdu_header = car(burst);
        pmt_t tx_time = dict_ref(pdu_header, intern("tx_time"),PMT_NIL);
        if(tx_time != PMT_NIL){
          pmt_t tx_command_time = cons(tuple_ref(tx_time,0),tuple_ref(tx_time,1));
          pmt_t header_plus_burst = cdr(burst);
          uint32_t frame_nr = 0;
          
          pmt_t fn = dict_ref(pdu_header,intern("fn"),PMT_NIL);
          if(fn == PMT_NIL){
            gsmtap_hdr *header = (gsmtap_hdr *)blob_data(header_plus_burst);
            uint32_t frame_nr = be32toh(header->frame_number);
          } else {
            frame_nr = to_uint64(fn);
          }
          
          int mai = calculate_ma_sfh(d_maio, d_hsn, d_ma.size(), frame_nr);
          uint16_t arfcn = d_ma[mai];
          
  //        if(fn == PMT_NIL){
  //          header->arfcn = htobe16(arfcn);
  //          header->arfcn = header->arfcn | 0x8000; // set uplink flag
  //        }
          
          //compute the frequences to be set in the burst header
          double freq_uplink   = static_cast<double>(gsm_arfcn2freq10(arfcn, 1)) * 1.0e5;
          double freq_downlink = static_cast<double>(gsm_arfcn2freq10(arfcn, 0)) * 1.0e5;
          
          pmt_t tx_command = dict_add(make_dict(),intern("lo_freq"),from_double(d_base_freq));
          tx_command = dict_add(tx_command,intern("dsp_freq"),from_double(freq_uplink-d_base_freq));
          tx_command = dict_add(tx_command,intern("time"),tx_command_time);

          pmt_t rx_command = dict_add(make_dict(),intern("lo_freq"),from_double(d_base_freq));
          rx_command = dict_add(rx_command,intern("dsp_freq"),from_double(freq_uplink-d_base_freq));
          rx_command = dict_add(rx_command,intern("time"),tx_command_time);
          rx_command = dict_add(rx_command,intern("direction"),intern("RX"));
          
          pdu_header = dict_add(pdu_header, intern("tx_command"),tx_command);
//          pdu_header = dict_add(pdu_header, intern("tx_command"),rx_command);
//          std::cout << "arfcn " << arfcn << " mai " << mai << " d_ma.size() " << d_ma.size() << " d_hsn " << d_hsn << std::endl;
          std::cout << "arfcn_uplink " << arfcn << std::endl;
//          std::cout << "freq_downlink " << freq_downlink << std::endl;
//          std::cout << "pdu_header " << pdu_header << std::endl;
//          std::cout << "size_header_plus_burst " << length(header_plus_burst) << std::endl;
          message_port_pub(mp("bursts_out"), cons(pdu_header,header_plus_burst));
        }
      } else {
        message_port_pub(mp("bursts_out"), burst);
      }
    }
    
    bool freq_hopping_impl::set_hopping_params(pmt_t hopping_params){
      bool retval = false;
      if(hopping_params != PMT_NIL){
        //set hopping parameters immediately
        pmt_t hsn = dict_ref(hopping_params, intern("hsn"), PMT_NIL);
        pmt_t maio = dict_ref(hopping_params, intern("maio"), PMT_NIL);;
        pmt_t ma = dict_ref(hopping_params, intern("ma"), PMT_NIL);
        
        if(is_vector(ma) && is_integer(hsn) && is_integer(maio)){  //TODO: checking the values
          d_hsn = to_uint64(hsn);
          d_maio = to_uint64(maio);
          d_ma.resize(length(ma));
          for(int i=0; i<length(ma); i++){
            d_ma[i] = to_uint64(vector_ref(ma,i));
          }
          retval = true;
        }
      }
      return retval;
    }
  } /* namespace gsm */
} /* namespace gr */
