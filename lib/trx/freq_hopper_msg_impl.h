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

#ifndef INCLUDED_GSM_FREQ_HOPPER_MSG_IMPL_H
#define INCLUDED_GSM_FREQ_HOPPER_MSG_IMPL_H

#include <grgsm/trx/freq_hopper_msg.h>
#include <grgsm/misc_utils/time_spec.h>
#include <grgsm/misc_utils/fn_time.h>
#include <pmt/pmt.h>


namespace gr {
  namespace gsm {

    class freq_hopper_msg_impl : public freq_hopper_msg
    {
     private:
      double d_samp_rate;
      double d_fc_rx;
      double d_fc_tx;
      bool d_rx_hopping;
      bool d_tx_hopping;
      double d_freq_change_period;
      bool d_rx_time_received;
      time_spec_t d_last_rx_time;
      time_spec_t d_current_time;
      uint64_t d_current_start_offset; //!!zmienic nazwe - ta zmienna okresla pozycje ostatniego rx_time
      uint64_t d_i;
      std::vector<double> d_f;
      double d_start_time;
      time_spec_t d_next_change_time;
      double d_change_advance;
      pmt::pmt_t d_rx_time_key;

//      bool d_hopping_enable; //if true block do the hopping, if not block just passes the bursts
      uint64_t d_hsn; //hopping sequence number
      uint64_t d_maio; //mobile allocation index offset
      std::vector<uint64_t> d_ma; //mobile allocation

      uint64_t d_current_fn;//!!

//      pmt::pmt_t d_hopping_cmd; //TODO: change this a std::map
//      void set_freq_metadata(pmt::pmt_t cmd);
//      bool set_hopping_params(pmt::pmt_t hopping_params);
      double get_dsp_freq(uint64_t fn);
     public:
      freq_hopper_msg_impl(
        double samp_rate,
        double start_fc_rx,
        double start_fc_tx,
        bool rx_hopping,
        bool tx_hopping,
        double freq_change_period);
      ~freq_hopper_msg_impl();

      int work(int noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);

//      void add_hopping_cmd(pmt::pmt_t hopping_cmd);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_FREQ_HOPPER_MSG_IMPL_H */
