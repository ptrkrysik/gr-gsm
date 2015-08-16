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
#include "universal_ctrl_chans_demapper_impl.h"
#include <grgsm/endian.h>
#include <grgsm/gsmtap.h>
#include <set>

#define BURST_SIZE 148

namespace gr {
  namespace gsm {

    universal_ctrl_chans_demapper::sptr
    universal_ctrl_chans_demapper::make(unsigned int timeslot_nr, const std::vector<int> &starts_fn_mod51, const std::vector<int> &channel_types)
    {
      return gnuradio::get_initial_sptr
        (new universal_ctrl_chans_demapper_impl(timeslot_nr, starts_fn_mod51, channel_types));
    }

    /*
     * The private constructor
     */
    universal_ctrl_chans_demapper_impl::universal_ctrl_chans_demapper_impl(unsigned int timeslot_nr, const std::vector<int> &starts_fn_mod51, const std::vector<int> &channel_types)
      : gr::block("universal_ctrl_chans_demapper",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
       d_timeslot(timeslot_nr)
              
    {
    
        
        for(int ii=0; ii<51; ii++)
        {
            d_starts_fn_mod51[ii]=0;
            d_channel_types[ii]=0;
	    d_subslots[ii] = 0;
	    d_subslots[ii+51] = 0;
        }
        
        std::vector<int>::const_iterator s;
        std::vector<int>::const_iterator ch_type;
        
        for(s=starts_fn_mod51.begin(), ch_type=channel_types.begin();s != starts_fn_mod51.end(); s++)
        {
            if((*s >= 0) and (*s <= (51-4)))
            {
                for(int ii=0; ii<4; ii++){
                    d_starts_fn_mod51[*s+ii] = *s;
                    if(ch_type!=channel_types.end())
                    {
                        d_channel_types[*s+ii] = *ch_type;
                    }
                }
                if(ch_type!=channel_types.end())
                {
                    ch_type++;
                }
            }
        }
        
        std::set<int> distinct_channels(channel_types.begin(), channel_types.end());
	std::set<int>::iterator it;
        unsigned int subslot;
	
	for (it=distinct_channels.begin(); it != distinct_channels.end(); it++)
	{
	    subslot = 0;
	    for(s=starts_fn_mod51.begin();s != starts_fn_mod51.end(); s++)
	    {
		if ((d_channel_types[*s] == *it) and (*s >= 0) and (*s <= (51-4)))
		{
		    for(int ii=0; ii<4; ii++)
		    {
			d_subslots[*s+ii] = subslot;
			d_subslots[*s+ii+51] = subslot;
		    }
		    subslot++;
		}
	    }
	    
	    if (*it == GSMTAP_CHANNEL_ACCH or 
		*it == (GSMTAP_CHANNEL_ACCH|GSMTAP_CHANNEL_SDCCH) or
		*it == (GSMTAP_CHANNEL_ACCH|GSMTAP_CHANNEL_SDCCH4) or
		*it == (GSMTAP_CHANNEL_ACCH|GSMTAP_CHANNEL_SDCCH8) or
		*it == (GSMTAP_CHANNEL_ACCH|GSMTAP_CHANNEL_TCH_F) or
		*it == (GSMTAP_CHANNEL_ACCH|GSMTAP_CHANNEL_TCH_H)
	    )
	    {
		for(s=starts_fn_mod51.begin();s != starts_fn_mod51.end(); s++)
		{
		    if ((d_channel_types[*s] == *it) and (*s >= 0) and (*s <= (51-4)))
		    {
			for(int ii=0; ii<4; ii++)
			{
			    d_subslots[*s+ii+51] = subslot;
			}
			subslot++;
		    }
		}
	    }
	}
        
        
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&universal_ctrl_chans_demapper_impl::filter_ctrl_chans, this, _1));
        message_port_register_out(pmt::mp("bursts"));
    }

    /*
     * Our virtual destructor.
     */
    universal_ctrl_chans_demapper_impl::~universal_ctrl_chans_demapper_impl()
    {
    }
    
    void universal_ctrl_chans_demapper_impl::filter_ctrl_chans(pmt::pmt_t msg)
    {
        pmt::pmt_t header_plus_burst = pmt::cdr(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_plus_burst);
            
        uint32_t frame_nr = be32toh(header->frame_number);
        uint32_t fn_mod51 = frame_nr % 51;
        uint32_t fn51_start = d_starts_fn_mod51[fn_mod51];
        uint32_t fn51_stop = fn51_start + 3;
        uint32_t ch_type = d_channel_types[fn_mod51];
        
        if(header->timeslot==d_timeslot){
            int8_t new_msg[sizeof(gsmtap_hdr)+BURST_SIZE];
            gsmtap_hdr * new_hdr = (gsmtap_hdr*)new_msg;
            memcpy(new_msg, header, sizeof(gsmtap_hdr)+BURST_SIZE);
            new_hdr->sub_type = ch_type;
            new_hdr->sub_slot = d_subslots[fn_mod51 + (51 * (frame_nr % 2))];
            pmt::pmt_t msg_binary_blob = pmt::make_blob(new_msg,sizeof(gsmtap_hdr)+BURST_SIZE);
            pmt::pmt_t msg_out = pmt::cons(pmt::PMT_NIL, msg_binary_blob);
	    
            if(fn_mod51>=fn51_start && fn_mod51<=fn51_stop)
            {
                uint32_t ii = fn_mod51 - fn51_start;
                d_frame_numbers[ii] = frame_nr;
                d_bursts[ii] = msg_out;
            }
            
            if(fn_mod51==fn51_stop)
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
                        message_port_pub(pmt::mp("bursts"), d_bursts[jj]);
                    }
                } 
            }
        }
    }
  } /* namespace gsm */
} /* namespace gr */

