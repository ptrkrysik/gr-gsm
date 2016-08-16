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

#ifndef INCLUDED_GRGSM_BER_CALC_IMPL_H
#define INCLUDED_GRGSM_BER_CALC_IMPL_H

#include <grgsm/BER_calc.h>
#include "decoding/cch.h"
#include "GSMFrameCoder.h"

using namespace std;

namespace gr {
  namespace grgsm {

  class BER_calc_impl : public BER_calc
  {
  private:
      // Nothing to declare in this block.

  public:
      BER_calc_impl();
      ~BER_calc_impl();
      int general_work(int noutput_items,
               gr_vector_int &ninput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);
  private:
      //      char [CONV_SIZE+DATA_BLOCK_SIZE];
      pmt::pmt_t demod;
      pmt::pmt_t decod;
      void ber_calc(pmt::pmt_t msg);

      float BER;
      uint totalBits;

    };

  } // namespace grgsm
} // namespace gr

#endif /* INCLUDED_GRGSM_BER_CALC_IMPL_H */

