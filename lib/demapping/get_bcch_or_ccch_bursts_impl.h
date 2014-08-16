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

#ifndef INCLUDED_GSM_GET_BCCH_OR_CCCH_BURSTS_IMPL_H
#define INCLUDED_GSM_GET_BCCH_OR_CCCH_BURSTS_IMPL_H

#include <gsm/demapping/get_bcch_or_ccch_bursts.h>

namespace gr {
  namespace gsm {

    class get_bcch_or_ccch_bursts_impl : public get_bcch_or_ccch_bursts
    {
     private:
         unsigned int d_fn51_start;
         uint32_t d_frame_numbers[4];     
         pmt::pmt_t d_bursts[4];                
     public:
        get_bcch_or_ccch_bursts_impl(unsigned int fn51_start);
        ~get_bcch_or_ccch_bursts_impl();
        void filter_ccch(pmt::pmt_t msg);
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_GET_BCCH_OR_CCCH_BURSTS_IMPL_H */

