/* -*- c++ -*- */
/*
 * @file
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
 */

#ifndef INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_IMPL_H
#define INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_IMPL_H

#include <grgsm/demapping/universal_ctrl_chans_demapper.h>
#include <vector>

namespace gr {
  namespace gsm {

    class universal_ctrl_chans_demapper_impl : public universal_ctrl_chans_demapper
    {
     private:
      unsigned int d_starts_fn_mod51[51];
      unsigned int d_channel_types[51];
      unsigned int d_subslots[102];
      unsigned int d_timeslot;
      uint32_t d_frame_numbers[4];
      pmt::pmt_t d_bursts[4];
     public:
      universal_ctrl_chans_demapper_impl(unsigned int timeslot_nr, const std::vector<int> &starts_fn_mod51, const std::vector<int> &channel_types);
      ~universal_ctrl_chans_demapper_impl();

      void filter_ctrl_chans(pmt::pmt_t msg);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_UNIVERSAL_CTRL_CHANS_DEMAPPER_IMPL_H */

