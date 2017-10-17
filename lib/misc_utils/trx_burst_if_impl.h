/* -*- c++ -*- */
/* @file
 * @author Vadim Yanitskiy <axilirator@gmail.com>
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

#ifndef INCLUDED_GRGSM_TRX_BURST_IF_IMPL_H
#define INCLUDED_GRGSM_TRX_BURST_IF_IMPL_H

#include <stddef.h>

#include <grgsm/gsmtap.h>
#include <grgsm/misc_utils/trx_burst_if.h>

namespace gr {
  namespace gsm {

    class trx_burst_if_impl : public trx_burst_if
    {
     private:
      udp_socket *d_data_sock;
      udp_socket *d_clck_sock;
      int d_ts_filter_tn;

      bool detect_rach(uint8_t *burst);
      void clck_ind_send(uint32_t frame_nr);
      void burst_pack(pmt::pmt_t msg, uint8_t *buf);

     public:
      trx_burst_if_impl(const std::string &remote_addr, int base_port);
      ~trx_burst_if_impl();

      /* Timeslot filter API */
      void ts_filter_set_tn(int tn);
      int ts_filter_get_tn(void);

      void handle_dl_burst(pmt::pmt_t msg);
      void handle_ul_burst(uint8_t *payload, size_t len);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GRGSM_TRX_BURST_IF_IMPL_H */

