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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "BER_calc_impl.h"

namespace gr {
  namespace grgsm {

    BER_calc::sptr
    BER_calc::make()
    {
      return gnuradio::get_initial_sptr
        (new BER_calc_impl());
    }

    /*
     * The private constructor
     */
    BER_calc_impl::BER_calc_impl()
      : gr::block("BER_calc",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        BER(0),
        totalBits(0)
    {

        //setup input/output ports
        message_port_register_in(pmt::mp("de"));
        set_msg_handler(pmt::mp("de"), boost::bind(&BER_calc_impl::ber_calc, this, _1));

    }

    /*
     * Our virtual destructor.
     */
    BER_calc_impl::~BER_calc_impl()
    {
    }

    void BER_calc_impl::ber_calc(pmt::pmt_t msg)
    {
        demod = car(msg);
        decod = cdr(msg);

        unsigned char *dmod_bits = (unsigned char*) pmt::blob_data(demod);
        unsigned char *dcod_bits = (unsigned char*) pmt::blob_data(decod);

        GSMFrameCoder enc;
        char *inbin[3];
        inbin[0] = NULL;
        inbin[1] = "-b";
        inbin[2] = (char*)dcod_bits;

        BitVector1 *encoded_bits = enc.func(3, inbin);
        // computing BER
        uint errs=0;
        for (int ss=0; ss<4 ; ss++)
        {
            for (int kk=0; kk<iBLOCK_SIZE; kk++)
            {
                if ((encoded_bits[ss][kk] != dmod_bits[ss*iBLOCK_SIZE+kk]))
                    errs++;
            }
        }
        BER = (BER*totalBits+double(errs))/(CONV_SIZE+totalBits);
        totalBits += CONV_SIZE;
        cout << "BER= \n" << BER << endl;

    }
  } /* namespace grgsm */
} /* namespace gr */

