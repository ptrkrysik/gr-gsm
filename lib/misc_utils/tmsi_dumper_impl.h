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

#ifndef INCLUDED_GSM_TMSI_DUMPER_IMPL_H
#define INCLUDED_GSM_TMSI_DUMPER_IMPL_H

#include <grgsm/misc_utils/tmsi_dumper.h>

namespace gr {
  namespace gsm {

    class tmsi_dumper_impl : public tmsi_dumper
    {
     private:
      void dump_tmsi(pmt::pmt_t msg);

     public:
      tmsi_dumper_impl();
      ~tmsi_dumper_impl();
    };
  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TMSI_DUMPER_IMPL_H */

