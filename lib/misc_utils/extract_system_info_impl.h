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

#ifndef INCLUDED_GSM_EXTRACT_SYSTEM_INFO_IMPL_H
#define INCLUDED_GSM_EXTRACT_SYSTEM_INFO_IMPL_H

#include <gsm/misc_utils/extract_system_info.h>



namespace gr {
  namespace gsm {

    class chan_info {
      public:
        unsigned int id;
        int8_t pwr_db;
        unsigned int arfcn;
        float freq;
        unsigned int lac;
        unsigned int cell_id;
        unsigned int mnc;
        
        chan_info() :  id(-1), pwr_db(0), arfcn(0), freq(0), lac(0), cell_id(0), mnc(0){}
        chan_info(const chan_info & info) : id(info.id), pwr_db(info.pwr_db), arfcn(info.arfcn), freq(info.freq), lac(info.lac), cell_id(info.cell_id), mnc(info.mnc){}
        ~chan_info(){}
    };


    struct compare_id {
        inline bool operator()(const chan_info &a, const chan_info &b) const
        {
            return a.id < b.id;
        }
    };
    struct compare_pwr {
        inline bool operator()(const chan_info &a, const chan_info &b) const
        {
            return a.pwr_db < b.pwr_db;
        }
    };


    class extract_system_info_impl : public extract_system_info
    {
     private:
      void process_bursts(pmt::pmt_t burst);
      void process_sysinfo(pmt::pmt_t msg);
      std::set<chan_info, compare_id> d_c0_channels;
      bool after_reset;
     public:
      virtual void show();
      virtual std::vector<int> get_chans();
      virtual std::vector<int> get_pwrs();
      virtual std::vector<int> get_lac();
      virtual std::vector<int> get_cell_id();
      virtual std::vector<int> get_mnc();
      virtual void reset();
      extract_system_info_impl();
      ~extract_system_info_impl();
    };
  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_EXTRACT_SYSTEM_INFO_IMPL_H */

