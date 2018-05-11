
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
          d_hopping_enable(false)
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
        pmt::pmt_t header_plus_burst = pmt::cdr(burst);
        gsmtap_hdr *header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);

        uint32_t frame_nr = be32toh(header->frame_number);
        
        int mai = calculate_ma_sfh(d_maio, d_hsn, d_ma.size(), frame_nr);
        uint16_t arfcn = d_ma[mai];
        header->arfcn = htobe16(arfcn);
        header->arfcn = header->arfcn | 0x8000; // set uplink flag

        //oblicz czestotliwosc i ustaw w headerze

      } else {
        message_port_pub(pmt::mp("bursts_out"), burst);
      }
    }
    
    bool freq_hopping_impl::set_hopping_params(pmt::pmt_t hopping_params){
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
