/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#ifndef INCLUDED_GSM_GET_BCCH_OR_CCCH_BURSTS_H
#define INCLUDED_GSM_GET_BCCH_OR_CCCH_BURSTS_H

#include <gsm/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace gsm {

    /*!
     * \brief <+description of block+>
     * \ingroup gsm
     *
     */
    class GSM_API get_bcch_or_ccch_bursts : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<get_bcch_or_ccch_bursts> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gsm::get_bcch_or_ccch_bursts.
       *
       * To avoid accidental use of raw pointers, gsm::get_bcch_or_ccch_bursts's
       * constructor is in a private implementation
       * class. gsm::get_bcch_or_ccch_bursts::make is the public interface for
       * creating new instances.
       */
      static sptr make(unsigned int fn51_start);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_GET_BCCH_OR_CCCH_BURSTS_H */

