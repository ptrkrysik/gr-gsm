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


#ifndef INCLUDED_GSM_FREQ_HOPPING_H
#define INCLUDED_GSM_FREQ_HOPPING_H

#include <grgsm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace gsm {

    /*!
     * \brief <+description of block+>
     * \ingroup gsm
     *
     */
    class GRGSM_API freq_hopping : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<freq_hopping> sptr;

      /*!
       * hopping_cmd is a pmt dictionary with following fields:
       * "cmd": string with type of command, possible values:
       *     -"start" - immediately start hopping or if there is "fn" 
       *                field defined start from frame number defined 
       *                by that field
       *     -"stop"  - immediately stop hopping or if there is "fn" 
       *                field defined start from frame number defined 
       *                by that field
       *     -"discard" - discard all queued "start_fn" and "stop_fn" 
       *                (this command doesn't require hopping parameters or frame number)
       * "hopping_params": dictionary with hopping parameters in following form:
       *     {"hsn": hopping_sequence_number(uint64),
       *      "ma":   mobile_allocation(list of arfcns),
       *      "maio": mobile_allocation_index_offset(uint64),
       *      "n_arfcn": number_of_arfcns_in_ma}
       *  "fn": frame number when to start or stop hopping
       */
      static sptr make(pmt::pmt_t hopping_cmd);
      virtual void add_hopping_cmd(pmt::pmt_t hopping_cmd=pmt::PMT_NIL) = 0;
    };
  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_FREQ_HOPPING_H */

