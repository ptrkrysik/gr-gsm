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

#ifndef INCLUDED_GRGSM_TRX_BURST_IF_H
#define INCLUDED_GRGSM_TRX_BURST_IF_H

#include <grgsm/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace gsm {

    /*!
     * \brief <+description of block+>
     * \ingroup grgsm
     *
     */
    class GRGSM_API trx_burst_if : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<trx_burst_if> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of grgsm::trx_burst_if.
       *
       * To avoid accidental use of raw pointers, grgsm::trx_burst_if's
       * constructor is in a private implementation
       * class. grgsm::trx_burst_if::make is the public interface for
       * creating new instances.
       */
      static sptr make(
        const std::string &remote_addr,
        const std::string &base_port);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GRGSM_TRX_BURST_IF_H */

