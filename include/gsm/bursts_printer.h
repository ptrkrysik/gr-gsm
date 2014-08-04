/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_GSM_BURSTS_PRINTER_H
#define INCLUDED_GSM_BURSTS_PRINTER_H

#include <gsm/api.h>
#include <gnuradio/block.h>
#include <string>
#include <vector>



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

namespace gr {
  namespace gsm {

    /*!
     * \brief <+description of block+>
     * \ingroup gsm
     *
     */
    class GSM_API bursts_printer : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<bursts_printer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gsm::bursts_printer.
       *
       * To avoid accidental use of raw pointers, gsm::bursts_printer's
       * constructor is in a private implementation
       * class. gsm::bursts_printer::make is the public interface for
       * creating new instances.
       */
      static sptr make();
    };

  } // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURSTS_PRINTER_H */

