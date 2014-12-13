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
#include <grgsm/endian.h>
#include <grgsm/gsmtap.h>
#include "get_bcch_or_ccch_bursts_impl.h"

namespace gr {
  namespace gsm {

    get_bcch_or_ccch_bursts::sptr
    get_bcch_or_ccch_bursts::make(unsigned int fn51_start)
    {
      return gnuradio::get_initial_sptr
        (new get_bcch_or_ccch_bursts_impl(fn51_start));
    }

    /*
     * The private constructor
     */
    get_bcch_or_ccch_bursts_impl::get_bcch_or_ccch_bursts_impl(unsigned int fn51_start)
      : gr::block("get_bcch_or_ccch_bursts",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
              d_fn51_start(fn51_start)
    {
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&get_bcch_or_ccch_bursts_impl::filter_ccch, this, _1));
        message_port_register_out(pmt::mp("bursts"));
    }

    /*
     * Our virtual destructor.
     */
    get_bcch_or_ccch_bursts_impl::~get_bcch_or_ccch_bursts_impl()
    {
    }

    void get_bcch_or_ccch_bursts_impl::filter_ccch(pmt::pmt_t msg)
    {
        pmt::pmt_t header_plus_burst = pmt::cdr(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);
        uint32_t frame_nr = be32toh(header->frame_number);

        uint32_t fn_mod51 = frame_nr % 51;
        int fn51_stop = d_fn51_start+3;

        if(header->timeslot==0){
            if(fn_mod51>=d_fn51_start && fn_mod51<=fn51_stop){
                uint32_t ii = fn_mod51-d_fn51_start;
                d_frame_numbers[ii]=frame_nr;
                d_bursts[ii] = msg;
            }
            
            if(fn_mod51==fn51_stop){
                //check for a situation where some BCCH bursts were lost
                //in this situation frame numbers won't be consecutive
                bool frames_are_consecutive = true;
                for(int jj=1; jj<4; jj++)
                {
                    if((d_frame_numbers[jj]-d_frame_numbers[jj-1])!=1){
                        frames_are_consecutive = false;
                    }
                }
                if(frames_are_consecutive)
                {
                    //send bursts to the output
                    for(int jj=0; jj<4; jj++)
                    {
                        message_port_pub(pmt::mp("bursts"), d_bursts[jj]);
                    }              
                } 
            }
        }
    }
  } /* namespace gsm */
} /* namespace gr */

