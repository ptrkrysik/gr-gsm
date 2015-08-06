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

#ifndef INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_IMPL_H
#define INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_IMPL_H

#include <grgsm/flow_control/burst_sdcch_subslot_filter.h>

namespace gr {
  namespace gsm {

    class burst_sdcch_subslot_filter_impl : public burst_sdcch_subslot_filter
    {
     private:
      // hardcoded subslots of the channels, both SDCCH and the associated SACCH
      // -1 means that the particular position in the frame is not SDCCH
      const int8_t d_subslots_sdcch4[102] = {
          -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0, 1, 1, 1, 1,-1,-1, 2, 2, 2, 2, 3, 3, 3, 3,-1,-1, 0, 0, 0, 0, 1, 1, 1, 1,-1,
          -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 0, 0, 0, 0, 1, 1, 1, 1,-1,-1, 2, 2, 2, 2, 3, 3, 3, 3,-1,-1, 2, 2, 2, 2, 3, 3, 3, 3,-1
      };
      const int8_t d_subslots_sdcch8[102] = {
          0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,-1,-1,-1,
          0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7,-1,-1,-1
      };
      subslot_filter_mode d_mode;
      unsigned int d_subslot;
     public:
      burst_sdcch_subslot_filter_impl(subslot_filter_mode mode, unsigned int subslot);
      ~burst_sdcch_subslot_filter_impl();
      void process_burst(pmt::pmt_t msg);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_IMPL_H */
