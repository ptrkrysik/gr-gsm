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
#include "tch_f_chans_demapper_impl.h"
#include <grgsm/endian.h>
#include <grgsm/gsmtap.h>

namespace gr {
  namespace gsm {

    tch_f_chans_demapper::sptr
    tch_f_chans_demapper::make(unsigned int timeslot_nr)
    {
      return gnuradio::get_initial_sptr
        (new tch_f_chans_demapper_impl(timeslot_nr));
    }

    /*
     * The private constructor
     * 
     */
    tch_f_chans_demapper_impl::tch_f_chans_demapper_impl(unsigned int timeslot_nr)
      : gr::block("tch_f_chans_demapper",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
       d_timeslot(timeslot_nr)
              
    {
        for(int ii=0; ii<13; ii++)
        {
            int startpos = ((int)(ii / 4) * 4);
            d_starts_fn_mod26[ii] = startpos;
            d_starts_fn_mod26[ii + 13] = startpos + 13;
        }
        
        d_bursts_stolen = false;

        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&tch_f_chans_demapper_impl::filter_tch_chans, this, _1));
        message_port_register_out(pmt::mp("tch_bursts"));
        message_port_register_out(pmt::mp("acch_bursts"));
    }

    /*
     * Our virtual destructor.
     */
    tch_f_chans_demapper_impl::~tch_f_chans_demapper_impl()
    {
    }
    
    void tch_f_chans_demapper_impl::filter_tch_chans(pmt::pmt_t msg)
    {
        pmt::pmt_t header_plus_burst = pmt::cdr(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);

        uint32_t frame_nr = be32toh(header->frame_number);
        uint32_t fn_mod26 = frame_nr % 26;
        uint32_t fn26_start = d_starts_fn_mod26[fn_mod26];
        uint32_t fn26_stop = fn26_start + 3;
        int8_t * burst_bits = (int8_t *)(pmt::blob_data(header_plus_burst))+sizeof(gsmtap_hdr);

        if(header->timeslot == d_timeslot){
            header->sub_type = GSMTAP_CHANNEL_TCH_F;
            
            if (fn_mod26 == 12 || fn_mod26 == 25)
            {
                header->sub_type = GSMTAP_CHANNEL_ACCH|GSMTAP_CHANNEL_TCH_F;
                
                // position of SACCH burst based on timeslot
                // see specification gsm 05.02
                uint32_t index;
                bool is_sacch = false;
                
                if (d_timeslot % 2 == 0 && fn_mod26 == 12)
                {
                    index = (((frame_nr - 12) / 26) - (d_timeslot / 2)) % 4;
                    is_sacch = true;
                }
                else if (d_timeslot % 2 == 1 && fn_mod26 == 25) 
                {
                    index = (((frame_nr - 25) / 26) - ((d_timeslot - 1) / 2)) % 4;
                    is_sacch = true;
                }
                
                if (is_sacch)
                {
                    d_bursts_sacch[index] = msg;
                    d_frame_numbers_sacch[index] = frame_nr;
                    
                    if (index == 3) 
                    {
                        //check for a situation where some bursts were lost
                        //in this situation frame numbers won't be consecutive
                        bool frames_are_consecutive = true;
                        for(int jj=1; jj<4; jj++)
                        {
                            if((d_frame_numbers_sacch[jj]-d_frame_numbers_sacch[jj-1]) != 26)
                            {
                                frames_are_consecutive = false;
                            }
                        }
                        if(frames_are_consecutive)
                        {
                            //send bursts to the output
                            for(int jj=0; jj<4; jj++)
                            {
                                message_port_pub(pmt::mp("acch_bursts"), d_bursts_sacch[jj]);
                            }
                        }
                    }
                }
            }
            else
            {
                if(fn_mod26>=fn26_start && fn_mod26<=fn26_stop)
                {
                    uint32_t ii = fn_mod26 - fn26_start;
                    d_frame_numbers[ii] = frame_nr;
                    d_bursts[ii] = msg;
                    
                    // check if stealing bits are set
                    // see gsm 05.03 
                    if (static_cast<int>(burst_bits[60]) == 1 && static_cast<int>(burst_bits[87]) == 1)
                    {
                        d_bursts_stolen = true;
                    }
                }
                
                if(fn_mod26==fn26_stop)
                {
                    //check for a situation where some bursts were lost
                    //in this situation frame numbers won't be consecutive
                    bool frames_are_consecutive = true;
                    
                    for(int jj=1; jj<4; jj++)
                    {
                        if((d_frame_numbers[jj]-d_frame_numbers[jj-1])!=1)
                        {
                            frames_are_consecutive = false;
                        }
                    }
                    if(frames_are_consecutive)
                    {
                        //send bursts to the output
                        for(int jj=0; jj<4; jj++)
                        {
                            if (d_bursts_stolen)
                            {
                                message_port_pub(pmt::mp("acch_bursts"), d_bursts[jj]);
                            }
                            else
                            {
                                message_port_pub(pmt::mp("tch_bursts"), d_bursts[jj]);
                            }
                        }
                        d_bursts_stolen = false;
                    } 
                }
            }
        }
    }
  } /* namespace gsm */
} /* namespace gr */

