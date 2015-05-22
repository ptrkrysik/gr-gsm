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

#ifndef INCLUDED_GSM_UNIVERSAL_TCH_CHANS_DEMAPPER_IMPL_H
#define INCLUDED_GSM_UNIVERSAL_TCH_CHANS_DEMAPPER_IMPL_H

#include <grgsm/demapping/tch_f_chans_demapper.h>

namespace gr {
  namespace gsm {

    class tch_f_chans_demapper_impl : public tch_f_chans_demapper
    {
     private:
      unsigned int d_timeslot;
      uint32_t d_frame_numbers[3][8];
      uint32_t d_frame_numbers_sacch[4];
      pmt::pmt_t d_bursts[3][8];
      pmt::pmt_t d_bursts_sacch[4];
      bool d_bursts_stolen[3];

     public:
      tch_f_chans_demapper_impl(unsigned int timeslot_nr);
      ~tch_f_chans_demapper_impl();

      void filter_tch_chans(pmt::pmt_t msg);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_UNIVERSAL_TCH_CHANS_DEMAPPER_IMPL_H */

