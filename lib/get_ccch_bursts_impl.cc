/* -*- c++ -*- */
/* 
 * Copyright 2014 <perper@o2.pl>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "get_ccch_bursts_impl.h"
#include <gsmtap.h>

namespace gr {
  namespace gsm {

    void get_ccch_bursts_impl::filter_ccch(pmt::pmt_t msg)
    {
        pmt::pmt_t header_blob = pmt::car(msg);
        pmt::pmt_t content = pmt::cdr(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_blob);
        uint32_t frame_nr = header->frame_number;
        uint32_t frame_numbers[4];
        uint32_t fn_mod51 = header->frame_number % 51;
        
        if(header->timeslot==0){
            if(fn_mod51>=2 && fn_mod51<=5){
                uint32_t ii = fn_mod51-2;
                frame_numbers[ii]=header->frame_number;
                d_msgs[ii] = msg;
            }
            
            if(fn_mod51==5){
                //check for a situation where some BCCH bursts were lost
                //in this situation frame numbers won't be consecutive
                bool frames_are_consecutive = true;
                for(int jj=1; jj<4; jj++){
                    if((frame_numbers[jj]-frame_numbers[jj-1])!=1){
                        frames_are_consecutive = false;
                    }
                }
                if(frames_are_consecutive){
                    //send bursts to the output
                    for(int jj=0; jj<4; jj++){
                        message_port_pub(pmt::mp("bursts_out"), d_msgs[jj]);
                    }                 
                }
            }
        }
    }

    get_ccch_bursts::sptr
    get_ccch_bursts::make()
    {
      return gnuradio::get_initial_sptr
        (new get_ccch_bursts_impl());
    }

    /*
     * The private constructor
     */
    get_ccch_bursts_impl::get_ccch_bursts_impl()
      : gr::block("get_ccch_bursts",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0))
    {
        message_port_register_in(pmt::mp("bursts_in"));
        set_msg_handler(pmt::mp("bursts_in"), boost::bind(&get_ccch_bursts_impl::filter_ccch, this, _1));
        message_port_register_out(pmt::mp("bursts_out"));
    }

    /*
     * Our virtual destructor.
     */
    get_ccch_bursts_impl::~get_ccch_bursts_impl()
    {
    }
    
  } /* namespace gsm */
} /* namespace gr */

