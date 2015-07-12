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
#include <grgsm/gsmtap.h>
#include <unistd.h>
#include <map>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <endian.h>
#include <boost/foreach.hpp>

#include "extract_system_info_impl.h"

namespace gr {
  namespace gsm {
    boost::mutex extract_mutex;
    void extract_system_info_impl::process_bursts(pmt::pmt_t msg)
    {
        pmt::pmt_t burst_plus_header_blob = pmt::cdr(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(burst_plus_header_blob);

        chan_info info;
        info.id = be16toh(header->arfcn);
        info.pwr_db = header->signal_dbm;

        boost::mutex::scoped_lock lock(extract_mutex);
        
        if(d_c0_channels.find(info.id) != d_c0_channels.end()){
            d_c0_channels[info.id].copy_nonzero_elements(info);
        } else {
            d_c0_channels[info.id] = info;
        }
    }
    
    void extract_system_info_impl::process_sysinfo(pmt::pmt_t msg){
        pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
        uint8_t * message_plus_header = (uint8_t *)pmt::blob_data(message_plus_header_blob);
        gsmtap_hdr * header = (gsmtap_hdr *)message_plus_header;
        uint8_t * msg_elements = (uint8_t *)(message_plus_header+sizeof(gsmtap_hdr));

        if(msg_elements[2]==0x1b){
            chan_info info;
            info.id = be16toh(header->arfcn);                            //take arfcn
            info.pwr_db = header->signal_dbm;
            info.cell_id = (msg_elements[3]<<8)+msg_elements[4];         //take cell id
            info.lac = (msg_elements[8]<<8)+msg_elements[9];             //take lac
            info.mnc = (msg_elements[7]>>4);                             //take mnc
            boost::mutex::scoped_lock lock(extract_mutex);
            if(d_c0_channels.find(info.id) != d_c0_channels.end()){
                d_c0_channels[info.id].copy_nonzero_elements(info);
            } else {
                d_c0_channels[info.id] = info;
            }
        }
        else if(msg_elements[2]==0x1c){
            chan_info info;
            info.id = be16toh(header->arfcn);                            //take arfcn
            info.pwr_db = header->signal_dbm;
            info.lac = (msg_elements[6]<<8)+msg_elements[7];            //take lac
            info.mnc = (msg_elements[5]>>4);                            //take mnc

            boost::mutex::scoped_lock lock(extract_mutex);
            if(d_c0_channels.find(info.id) != d_c0_channels.end()){
                d_c0_channels[info.id].copy_nonzero_elements(info);
            } else {
                d_c0_channels[info.id] = info;
            }
        } else if(msg_elements[2]==0x1a){ //System Information Type 2
            chan_info info;
            info.id = be16toh(header->arfcn);                            //take arfcn
            info.pwr_db = header->signal_dbm;
            boost::mutex::scoped_lock lock(extract_mutex);
            //read neighbour cells
            decode_neighbour_cells(msg_elements, 3, info.id);
//            unsigned int    byte_nr, arfcn, bit;
//            arfcn=124;
//            bit=4;
//            for (byte_nr = 0; byte_nr <= 15; byte_nr++)            {
//                while (bit-- != 0)                {
//                    if (((msg_elements[byte_nr+3] >> bit) & 1) == 1){
//                        d_c0_channels[info.id].neighbour_cells.insert(arfcn);
//                    }
//                    arfcn--;
//                }
//                bit=8;
//            }
            
            if(d_c0_channels.find(info.id) != d_c0_channels.end()){
                d_c0_channels[info.id].copy_nonzero_elements(info);
            } else {
                d_c0_channels[info.id] = info;
            }
        }
    }
    
    void extract_system_info_impl::decode_neighbour_cells(uint8_t * data, unsigned int offset, unsigned int chan_id)
    {
        unsigned int  curr_offset, byte, bit, arfcn, format;
        uint8_t       oct;
        curr_offset = offset;
        format = data[curr_offset];

        if ((format & 0xc0) == 0x00)
        {
            /* bit map 0 */
            bit = 4;
            arfcn = 125;
            for (byte = 0; byte <= 15; byte++)
            {
                oct = data[curr_offset];
                while (bit-- != 0)
                {
                    arfcn--;
                    if (((oct >> bit) & 1) == 1)
                    {
                        d_c0_channels[chan_id].neighbour_cells.insert(arfcn);
                    }
                }
                bit = 8;
                curr_offset++;
            }
        }
        else if ((format & 0xc8) == 0x80)
        {
            /* 1024 range */
//            dissect_channel_list_n_range(tvb, tree, pinfo, curr_offset, len, 1024);
        }
        else if ((format & 0xce) == 0x88)
        {
            /* 512 range */
//            dissect_channel_list_n_range(tvb, tree, pinfo, curr_offset, len, 512);
        }
        else if ((format & 0xce) == 0x8a)
        {
            /* 256 range */
//            dissect_channel_list_n_range(tvb, tree, pinfo, curr_offset, len, 256);
        }
        else if ((format & 0xce) == 0x8c)
        {
            /* 128 range */
//            dissect_channel_list_n_range(tvb, tree, pinfo, curr_offset, len, 128);
        }
        else if ((format & 0xce) == 0x8e)
        {
            /* variable bit map */
            arfcn = ((format & 0x01) << 9) | (data[curr_offset+1] << 1) | ((data[curr_offset+2] & 0x80) >> 7);
            curr_offset = curr_offset + 2;
            bit = 7;
            for(byte = 0; byte <= (15-2); byte++){
                oct = data[curr_offset];
                while (bit-- != 0){
                    arfcn++;
                    if (((oct >> bit) & 1) == 1){
                        d_c0_channels[chan_id].neighbour_cells.insert(arfcn);
                    }
                }
                bit = 8;
                curr_offset++;
            }
        }

        return;
    }
    
//    void extract_system_info_impl::dissect_channel_list_n_range(guint32 offset, guint len, gint range)
//    {
//        int        curr_offset = offset, bit_offset, f0, arfcn_orig, w[64], wsize, i;
//        int        octet, nwi  = 1, jwi=0, imax, iused, arfcn;
//        uint8_t      list[1024];

//        memset((void*)list,0,sizeof(list));

////        subtree = proto_tree_add_subtree_format(tree,tvb, curr_offset, len,
////                                                ett_gsm_rr_elem[DE_RR_NEIGH_CELL_DESC], NULL, "Range %d format", range);

//        octet = data[curr_offset];
//        if (range == 1024) {
//            f0 = (octet>>2)&1;
//            if (f0)
//                list[0] = 1;
//            arfcn_orig = 0;
//            wsize = 10;
//            imax = 16;
//            bit_offset = curr_offset*8 + 6;
//        }
//        else {
//            bit_offset = curr_offset*8 + 7;
//            arfcn_orig = (gint) tvb_get_bits(tvb, bit_offset, 10, FALSE);
////            proto_tree_add_bits_item(subtree, hf_n_range_orig_arfcn, tvb, bit_offset, 10, ENC_BIG_ENDIAN);
//            bit_offset+=10;

//            list[arfcn_orig] = 1;

//            switch (range) {
//            case 512:
//                wsize=9;
//                imax = 17;
//                break;
//            case 256:
//                wsize=8;
//                imax = 21;
//                break;
//            case 128:
//                wsize=7;
//                imax = 28;
//                break;
//            default:
////                DISSECTOR_ASSERT_NOT_REACHED();
//            }
//        }
//        iused = imax;   /* in case the list is actually full */

//        /* extract the variable size w[] elements */
//        for (i=1; i<=imax; i++) {
//            w[i] = (gint) tvb_get_bits(tvb, bit_offset, wsize, FALSE);
////            proto_tree_add_text(subtree, tvb, bit_offset>>3, ((bit_offset+wsize-1)>>3) - (bit_offset>>3) + 1 , "%s %s(%d): %d",
////                                decode_bits_in_field(bit_offset, wsize, w[i]),
////                                "W",
////                                i,
////                                w[i]);
//            bit_offset += wsize;
//            curr_offset = bit_offset>>3;

//            if ((iused == imax) && (w[i] == 0) ) {
//                iused = i - 1;
//            }
//            if ((curr_offset-offset)>len) {
//                iused = i - 1;
//                break;
//            }
//            if (++jwi == nwi) {       /* check if the number of wi at this wsize has been extracted */
//                jwi = 0;            /* reset the count of wi at this size */
//                nwi <<= 1;          /* get twice as many of the next size */
//                wsize--;            /* make the next size 1 bit smaller */
//            }
//        }

//        for (i=1; i<=iused; i++) {
//            arfcn = (f_k(i, w, range) + arfcn_orig)%1024;
//            list[arfcn] = 1;
//        }

////        display_channel_list(list, tvb, tree, offset, curr_offset-offset);

//        return;
//    }
    
    std::vector<int> extract_system_info_impl::get_chans()
    {
        std::vector<int> chans_ids;
        BOOST_FOREACH(chan_info_map::value_type &i, d_c0_channels){
            chans_ids.push_back(i.second.id);
        }
        return chans_ids;
    }
    
    std::vector<int> extract_system_info_impl::get_lac()
    {
        std::vector<int> lacs;
        BOOST_FOREACH(chan_info_map::value_type &i, d_c0_channels){
            lacs.push_back(i.second.lac);
        }
        return lacs;
    }
    
    std::vector<int> extract_system_info_impl::get_mnc()
    {
        std::vector<int> mncs;
        BOOST_FOREACH(chan_info_map::value_type &i, d_c0_channels){
            mncs.push_back(i.second.mnc);
        }
        return mncs;
    }
    
    std::vector<int> extract_system_info_impl::get_cell_id()
    {
        std::vector<int> cell_ids;
        BOOST_FOREACH(chan_info_map::value_type &i, d_c0_channels){
            cell_ids.push_back(i.second.cell_id);
        }
        return cell_ids;
    }
    
    std::vector<int> extract_system_info_impl::get_pwrs()
    {
        std::vector<int> pwrs;
        BOOST_FOREACH(chan_info_map::value_type &i, d_c0_channels){
            pwrs.push_back(i.second.pwr_db);
        }
        return pwrs;
    }
    std::vector<int> extract_system_info_impl::get_neighbours(int chan_id)
    {
        std::vector<int> neighbour_cells;
        BOOST_FOREACH(int n, d_c0_channels[chan_id].neighbour_cells){
            neighbour_cells.push_back(n);
        }
        return neighbour_cells;
    }
    
    void extract_system_info_impl::reset()
    {
        d_c0_channels.clear();
        if(!empty_p(pmt::mp("bursts"))){
            delete_head_blocking(pmt::mp("bursts"));
        }
        if(!empty_p(pmt::mp("msgs"))){
            delete_head_blocking(pmt::mp("msgs"));
        }
    }
    
    extract_system_info::sptr
    extract_system_info::make()
    {
      return gnuradio::get_initial_sptr
        (new extract_system_info_impl());
    }

    /*
     * The private constructor
     */
    extract_system_info_impl::extract_system_info_impl()
      : gr::block("extract_system_info",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)), 
              after_reset(false)
    {
        message_port_register_in(pmt::mp("bursts"));
        set_msg_handler(pmt::mp("bursts"), boost::bind(&extract_system_info_impl::process_bursts, this, _1));
        message_port_register_in(pmt::mp("msgs"));
        set_msg_handler(pmt::mp("msgs"), boost::bind(&extract_system_info_impl::process_sysinfo, this, _1));
    }
    
    /*
     * Our virtual destructor.
     */
    extract_system_info_impl::~extract_system_info_impl()
    {
    }


  } /* namespace gsm */
} /* namespace gr */

