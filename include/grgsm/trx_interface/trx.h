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

#ifndef INCLUDED_GRGSM_TRX_H
#define INCLUDED_GRGSM_TRX_H

#include <grgsm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace grgsm {

    /*!
     * \brief <+description of block+>
     * \ingroup grgsm
     *
     */
    class GRGSM_API trx : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<trx> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of grgsm::trx.
       *
       * To avoid accidental use of raw pointers, grgsm::trx's
       * constructor is in a private implementation
       * class. grgsm::trx::make is the public interface for
       * creating new instances.
       */
      static sptr make(
        const std::string &remote_addr,
        const std::string &base_port);

      /* Expose internal timeslot filter API */
      virtual void ts_filter_set_tn(int tn) = 0;
      virtual int ts_filter_get_tn(void) = 0;
    };

  } // namespace grgsm
} // namespace gr

#endif /* INCLUDED_GRGSM_TRX_H */

