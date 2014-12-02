/* -*- c++ -*- */
/*
 * @file
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
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "controlled_rotator_cc_impl.h"
#include <math.h>

namespace gr {
  namespace gsm {

    controlled_rotator_cc::sptr
    controlled_rotator_cc::make(double phase_inc, double samp_rate)
    {
      return gnuradio::get_initial_sptr
        (new controlled_rotator_cc_impl(phase_inc, samp_rate));
    }

    /*
     * The private constructor
     */
    controlled_rotator_cc_impl::controlled_rotator_cc_impl(double phase_inc, double samp_rate)
      : gr::sync_block("controlled_rotator_cc",
              gr::io_signature::make2(1, 2, sizeof(gr_complex), sizeof(float)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)))
    {
      set_phase_inc(phase_inc);
      set_samp_rate(samp_rate);
    }
    
    /*
     * Our virtual destructor.
     */
    controlled_rotator_cc_impl::~controlled_rotator_cc_impl()
    {
    }

    void
    controlled_rotator_cc_impl::set_phase_inc(double phase_inc)
    {
      d_phase_inc = phase_inc;
      d_r.set_phase_incr( exp(gr_complex(0, (double)phase_inc)) );
    }

    void
    controlled_rotator_cc_impl::set_samp_rate(double samp_rate)
    {
      d_samp_rate = samp_rate;
    }

    int
    controlled_rotator_cc_impl::work(int noutput_items,
			  gr_vector_const_void_star &input_items,
			  gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *)input_items[0];
      gr_complex *out = (gr_complex *)output_items[0];

      if(input_items.size() == 2) {
        int ii=0;
        const float *pp = (const float *)input_items[1];
        
        while(ii < noutput_items){
          //look for different values on phase increment control input
          if(d_phase_inc != (*pp)){

            set_phase_inc(*(pp));      //set new value of phase increment
            
            float freq_offset_setting = (*(pp) / (2*M_PI)) * d_samp_rate; //send stream tag with a new value of the frequency offset

            uint64_t offset = nitems_written(0);
            pmt::pmt_t key = pmt::string_to_symbol("setting_freq_offset");
            pmt::pmt_t value =  pmt::from_double(freq_offset_setting);
            add_item_tag(0,offset, key, value);
    
            break;
          }
          pp++;
          ii++;
        }
      }
      d_r.rotateN(out, const_cast<gr_complex *>(in), noutput_items); //const_cast<gr_complex *> is workaround old implementation of rotateN that is still present in ubuntu 14.04 packages
      return noutput_items;
    }

  } /* namespace gsm */
} /* namespace gr */

