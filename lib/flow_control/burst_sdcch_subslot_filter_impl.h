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
