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

#include "freq_hopper_msg_impl.h"
#include "../misc_utils/freq_hopping_utils.h"
extern "C" {
#include <osmocom/gsm/gsm_utils.h>
}

namespace gr {
  namespace gsm {
    using namespace pmt;

    freq_hopper_msg::sptr
    freq_hopper_msg::make(
      double samp_rate,
      double start_fc_rx,
      double start_fc_tx,
      bool rx_hopping,
      bool tx_hopping,
      double freq_change_period)
    {
      return gnuradio::get_initial_sptr
        (new freq_hopper_msg_impl(samp_rate, start_fc_rx, start_fc_tx,
          rx_hopping, tx_hopping, freq_change_period));
    }

    /*
     * The private constructor
     */
    freq_hopper_msg_impl::freq_hopper_msg_impl(
      double samp_rate,
      double start_fc_rx,
      double start_fc_tx,
      bool rx_hopping,
      bool tx_hopping,
      double freq_change_period) :
        sync_block("freq_hopper_msg",
          gr::io_signature::make(1, 1, sizeof(gr_complex)),
          gr::io_signature::make(0, 0, 0)),
        d_samp_rate(samp_rate),
        d_fc_rx(start_fc_rx),
        d_fc_tx(start_fc_tx),
        d_rx_hopping(rx_hopping),
        d_tx_hopping(tx_hopping),
        d_freq_change_period(freq_change_period),
        d_rx_time_received(false),
        d_last_rx_time(0.0),
        d_current_time(0.0),
        d_current_start_offset(0.0),
        d_i(0),
        d_start_time(1.0),
        d_next_change_time(time_spec_t(2.0)),
        d_change_advance(0.15),
        d_rx_time_key(string_to_symbol("rx_time"))
//          d_hopping_cmd(PMT_NIL),
//          d_hopping_enable(false),
//          d_base_freq(890e6)
    {
        // Register I/O ports
//        message_port_register_in(mp("hopping_cmd"));
        message_port_register_out(mp("control"));
//      d_f.push_back(-0.3e6);
//      d_f.push_back(-0.2e6);
//      d_f.push_back(-0.1e6);
//      d_f.push_back(0);
//      d_f.push_back(0.1e6);
//      d_f.push_back(0.2e6);
//      d_f.push_back(0.3e6);

      d_ma.push_back(51);
      d_ma.push_back(2);
      d_ma.push_back(37);
      d_ma.push_back(45);
      d_hsn = 0;
      d_maio = 0;
      d_current_fn = 0;
//        // Bind message handlers
//        set_msg_handler(mp("hopping_cmd"),
//          boost::bind(&freq_hopper_msg_impl::add_hopping_cmd,
//            this, _1));

//        add_hopping_cmd(hopping_cmd);
    }

    /*
     * Our virtual destructor.
     */
    freq_hopper_msg_impl::~freq_hopper_msg_impl()
    {
    }

    int freq_hopper_msg_impl::work(
      int noutput_items,
      gr_vector_const_void_star &input_items,
      gr_vector_void_star &output_items)
    {
      std::vector<tag_t> rx_time_tags;
      get_tags_in_window(rx_time_tags, 0, 0, noutput_items, d_rx_time_key);
      if(rx_time_tags.size() > 0){
        tag_t rx_time_tag = rx_time_tags[0];
        uint64_t rx_time_full_part = to_uint64(tuple_ref(rx_time_tag.value,0));
        double rx_time_frac_part = to_double(tuple_ref(rx_time_tag.value,1));
        d_last_rx_time = time_spec_t(rx_time_full_part, rx_time_frac_part);
        d_current_start_offset = rx_time_tag.offset;
        d_rx_time_received = true;
      }
      if(d_rx_time_received){
        uint64_t buffer_end_offset = nitems_read(0) + noutput_items;
        d_current_time = time_spec_t::from_ticks(buffer_end_offset - d_current_start_offset, d_samp_rate) + d_last_rx_time;
      }

//    Freq. control messages generation
      if ((d_current_time.get_real_secs() > d_start_time) &&
          ((d_next_change_time-d_current_time).get_real_secs() < d_change_advance)){
        pmt_t freq_change_time_pmt = cons(
          from_uint64(d_next_change_time.get_full_secs()),
          from_double(d_next_change_time.get_frac_secs())
        );
        d_next_change_time = d_next_change_time + d_freq_change_period;
        double dsp_freq = get_dsp_freq(d_current_fn);
        if(d_tx_hopping){
          pmt_t tx_tune_msg = dict_add(make_dict(), mp("lo_freq"), from_double(d_fc_tx));
//          tx_tune_msg = dict_add(tx_tune_msg, mp("dsp_freq"), from_double(d_f[d_i % d_f.size()]));
          tx_tune_msg = dict_add(tx_tune_msg, mp("dsp_freq"), from_double(dsp_freq));
          tx_tune_msg = dict_add(tx_tune_msg, mp("time"), freq_change_time_pmt);
          tx_tune_msg = dict_add(tx_tune_msg, mp("direction"), mp("TX"));
          message_port_pub(mp("control"), tx_tune_msg);
//          std::cout << "tx: " << tx_tune_msg << std::endl;
        }
        if(d_rx_hopping){
          pmt_t rx_tune_msg = dict_add(make_dict(), mp("lo_freq"), from_double(d_fc_rx));
//          rx_tune_msg = dict_add(rx_tune_msg, mp("dsp_freq"), from_double(-d_f[d_i % d_f.size()]));
          rx_tune_msg = dict_add(rx_tune_msg, mp("dsp_freq"), from_double(-dsp_freq));
          rx_tune_msg = dict_add(rx_tune_msg, mp("time"), freq_change_time_pmt);
          rx_tune_msg = dict_add(rx_tune_msg, mp("direction"), mp("RX"));
          message_port_pub(mp("control"), rx_tune_msg);
//          std::cout << "rx: " << rx_tune_msg << std::endl;
        }
        d_i++;
        d_current_fn++;
      }
      return noutput_items;
    }

    double freq_hopper_msg_impl::get_dsp_freq(uint64_t fn){ //TODO: jak otrzymac fn
      int mai = calculate_ma_sfh(d_maio, d_hsn, d_ma.size(), fn);
      uint16_t arfcn = d_ma[mai];
      double dsp_freq = static_cast<double>(gsm_arfcn2freq10(arfcn, 0)) * 1.0e5 - d_fc_rx;
    }
/*
    void freq_hopper_msg_impl::add_hopping_cmd(pmt_t cmd) //TODO: fn and discard not supported at the moment
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

    void freq_hopper_msg_impl::set_freq_metadata(pmt_t burst)
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
    
    bool freq_hopper_msg_impl::set_hopping_params(pmt_t hopping_params){
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
    }*/

  } /* namespace gsm */
} /* namespace gr */
