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


#ifndef INCLUDED_GRGSM_BER_CALC_H
#define INCLUDED_GRGSM_BER_CALC_H

#include <grgsm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace grgsm {

    /*!
     * \brief It is to calculate the BER of the demodulated bits. This is done by:
     *  1) encode the decoded bits of control channels
     *  2) count the difference between demodulated and encoded bits
     *
     * \attention The decoded bits comming from "control_channels_decoder" are parity checked,
     * hence they are captured correctly. Therefore, by encoding them again, the correct modulated
     * bits can be obtained.
     *
     * \ingroup grgsm
     *
     */
    class GRGSM_API BER_calc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<BER_calc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of grgsm::BER_calc.
       *
       * To avoid accidental use of raw pointers, grgsm::BER_calc's
       * constructor is in a private implementation
       * class. grgsm::BER_calc::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace grgsm
} // namespace gr

#endif /* INCLUDED_GRGSM_BER_CALC_H */

