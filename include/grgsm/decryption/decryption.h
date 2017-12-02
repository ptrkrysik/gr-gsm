/* -*- c++ -*- */
/*
 *  @file
 *  @author (C) 2014 by Piotr Krysik <ptrkrysik@gmail.com>
 *  @section LICENSE
 *
 *  Gr-gsm is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3, or (at your option)
 *  any later version.
 *
 *  Gr-gsm is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  *
 *  You should have received a copy of the GNU General Public License
 *  along with gr-gsm; see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street,
 *  Boston, MA 02110-1301, USA.
 *
 */


#ifndef INCLUDED_GSM_DECRYPTION_H
#define INCLUDED_GSM_DECRYPTION_H

#include <grgsm/api.h>
#include <gnuradio/block.h>
#include <vector>

namespace gr {
  namespace gsm {

    /*!
     * \brief <+description of block+>
     * \ingroup gsm
     *
     */
    class GRGSM_API decryption : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<decryption> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gsm::decryption.
       *
       * To avoid accidental use of raw pointers, gsm::decryption's
       * constructor is in a private implementation
       * class. gsm::decryption::make is the public interface for
       * creating new instances.
       */
      static sptr make(const std::vector<uint8_t> & k_c, unsigned int a5_version);
      virtual void set_k_c(const std::vector<uint8_t> & k_c) = 0;
      virtual void set_a5_version(unsigned int a5_version) = 0;
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_DECRYPTION_H */

