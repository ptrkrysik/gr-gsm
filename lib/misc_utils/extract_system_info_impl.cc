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
#include <gsm/gsmtap.h>
#include <unistd.h>
#include <set>
#include <iterator>
#include <algorithm>
#include <iostream>

#include "extract_system_info_impl.h"

namespace gr {
  namespace gsm {
    boost::mutex extract_mutex;
    void extract_system_info_impl::process_bursts(pmt::pmt_t msg)
    {
        pmt::pmt_t burst = pmt::cdr(msg);
        int8_t * burst_elements = (int8_t *)pmt::blob_data(burst);
        size_t burst_len=pmt::blob_length(burst);

        pmt::pmt_t header_blob = pmt::car(msg);
        gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_blob);
        chan_info info;
        info.id = header->arfcn;
        info.pwr_db = header->signal_dbm;
        
        std::set<chan_info, compare_id>::iterator iter = d_c0_channels.find(info);

        boost::mutex::scoped_lock lock(extract_mutex);
        if(iter != d_c0_channels.end()){
            info.lac = iter->lac;
            info.cell_id = iter->cell_id;
            info.mnc = iter->mnc;
            d_c0_channels.erase(iter);
            d_c0_channels.insert(info);
        } 
        d_c0_channels.insert(info);
    }
    
    void extract_system_info_impl::process_sysinfo(pmt::pmt_t msg){
        pmt::pmt_t msg_blob = pmt::cdr(msg);
        uint8_t * msg_elements = (uint8_t *)pmt::blob_data(msg_blob);

        if(msg_elements[2]==0x1b){
            //wyciągnij arfcn
            pmt::pmt_t header_blob = pmt::car(msg);
            gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_blob);
            chan_info info;
            info.id = header->arfcn;
            info.pwr_db = header->signal_dbm;
            info.cell_id = (msg_elements[3]<<8)+msg_elements[4];             //wyciągnij cell id
            info.lac = (msg_elements[8]<<8)+msg_elements[9];             //wyciągnij lac
            info.mnc = (msg_elements[7]>>4);    //wyciągnij id operatora

            std::set<chan_info, compare_id>::iterator iter = d_c0_channels.find(info);
            boost::mutex::scoped_lock lock(extract_mutex);
            if(iter != d_c0_channels.end()){
                d_c0_channels.erase(iter);
            }
            d_c0_channels.insert(info);
        }
        else if(msg_elements[2]==0x1c){
            pmt::pmt_t header_blob = pmt::car(msg);
            gsmtap_hdr * header = (gsmtap_hdr *)pmt::blob_data(header_blob);
            chan_info info;
            info.id = header->arfcn;
            info.pwr_db = header->signal_dbm;
            info.lac = (msg_elements[6]<<8)+msg_elements[7];             //wyciągnij lac
            info.mnc = (msg_elements[5]>>4);    //wyciągnij id operatora

            std::set<chan_info, compare_id>::iterator iter = d_c0_channels.find(info);
            boost::mutex::scoped_lock lock(extract_mutex);
            if(iter != d_c0_channels.end()){
                d_c0_channels.erase(iter);
                if(iter->cell_id!=0){
                    info.cell_id=iter->cell_id;
                }
            }
            d_c0_channels.insert(info);
        }
    }
    
    void extract_system_info_impl::show(){
        std::vector<chan_info> chans(d_c0_channels.begin(), d_c0_channels.end()); 
        std::vector<chan_info>::iterator iter;
        //std::sort(chans.begin(), chans.end(), compare_pwr());
        for(iter=chans.begin(); iter != chans.end(); ++iter){
            std::cout << static_cast<int>((*iter).id) << "(" << static_cast<int>((*iter).pwr_db) << ") ";
        }
        std::cout << std::endl;
    }
    
    std::vector<int> extract_system_info_impl::get_chans()
    {
        std::vector<chan_info> chans(d_c0_channels.begin(), d_c0_channels.end()); 
        std::vector<int> chans_ids(chans.size(),-1);
        //std::sort(chans.begin(), chans.end(), compare_pwr());

        for(int ii; ii < chans.size(); ++ii){
            chans_ids[ii] = chans[ii].id;
        }
        
        return chans_ids;
    }
    
    std::vector<int> extract_system_info_impl::get_lac()
    {
        std::vector<chan_info> chans(d_c0_channels.begin(), d_c0_channels.end()); 
        std::vector<int> chans_ids(chans.size(),-1);
        //std::sort(chans.begin(), chans.end(), compare_pwr());

        for(int ii; ii < chans.size(); ++ii){
            chans_ids[ii] = chans[ii].lac;
        }
        
        return chans_ids;
    }
    std::vector<int> extract_system_info_impl::get_mnc()
    {
        std::vector<chan_info> chans(d_c0_channels.begin(), d_c0_channels.end()); 
        std::vector<int> chans_ids(chans.size(),-1);
        //std::sort(chans.begin(), chans.end(), compare_pwr());

        for(int ii; ii < chans.size(); ++ii){
            chans_ids[ii] = chans[ii].mnc;
        }
        
        return chans_ids;
    }
    std::vector<int> extract_system_info_impl::get_cell_id()
    {
        std::vector<chan_info> chans(d_c0_channels.begin(), d_c0_channels.end()); 
        std::vector<int> chans_ids(chans.size(),-1);
        //std::sort(chans.begin(), chans.end(), compare_pwr());

        for(int ii; ii < chans.size(); ++ii){
            chans_ids[ii] = chans[ii].cell_id;
        }
        
        return chans_ids;
    }
    
    std::vector<int> extract_system_info_impl::get_pwrs()
    {
        std::vector<chan_info> chans(d_c0_channels.begin(), d_c0_channels.end()); 
        std::vector<int> pwrs(chans.size(),-1);
        //std::sort(chans.begin(), chans.end(), compare_pwr());

        for(int ii; ii < chans.size(); ++ii){
            pwrs[ii] = chans[ii].pwr_db;
        }
        
        return pwrs;
    }
    
    void extract_system_info_impl::reset()
    {
        std::set<chan_info, compare_id>::iterator iter;
        
        chan_info info;

        for(iter = d_c0_channels.begin(); iter != d_c0_channels.end(); iter++){
            info.id = iter->id;
            info.cell_id = iter->cell_id;             //wyciągnij cell id
            info.lac = iter->lac;            //wyciągnij lac
            info.mnc = iter->mnc;
            info.pwr_db = -111;
            d_c0_channels.erase(iter);
            d_c0_channels.insert(info);
        }
//        d_c0_channels.clear();
        
        if(!empty_p(pmt::mp("bursts"))){
            delete_head_blocking(pmt::mp("bursts"));
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

