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

#ifndef INCLUDED_GSM_FREQ_HOPPING_IMPL_H
#define INCLUDED_GSM_FREQ_HOPPING_IMPL_H

#include <grgsm/trx/freq_hopping.h>
#include <grgsm/misc_utils/time_spec.h>
#include <grgsm/misc_utils/fn_time.h>

namespace gr {
  namespace gsm {

    class freq_hopping_impl : public freq_hopping
    {
     private:
      bool d_hopping_enable; //if true block do the hopping, if not block just passes the bursts
      uint64_t d_hsn; //hopping sequence number
      uint64_t d_maio; //mobile allocation index offset
      double d_base_freq; //local oscillator frequency
      std::vector<uint64_t> d_ma; //mobile allocation
     
      pmt::pmt_t d_hopping_cmd; //TODO: change this uint64_to a std::map
      void set_freq_metadata(pmt::pmt_t cmd);
      bool set_hopping_params(pmt::pmt_t hopping_params);
     public:
      freq_hopping_impl(pmt::pmt_t hopping_cmd);
      ~freq_hopping_impl();

      void add_hopping_cmd(pmt::pmt_t hopping_cmd);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_FREQ_HOPPING_IMPL_H */
