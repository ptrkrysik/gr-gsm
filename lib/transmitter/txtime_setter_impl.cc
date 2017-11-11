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
#include <grgsm/gsmtap.h>
#include "txtime_setter_impl.h"

namespace gr {
  namespace gsm {

    txtime_setter::sptr
    txtime_setter::make(uint32_t init_fn, uint64_t init_time_secs, double init_time_fracs, uint64_t time_hint_secs, double time_hint_fracs, double timing_advance, double delay_correction)
    {
      return gnuradio::get_initial_sptr
        (new txtime_setter_impl(init_fn, init_time_secs, init_time_fracs, time_hint_secs, time_hint_fracs, timing_advance, delay_correction));
    }

    /*
     * The private constructor
     */
    txtime_setter_impl::txtime_setter_impl(uint32_t init_fn, uint64_t init_time_secs, double init_time_fracs, uint64_t time_hint_secs, double time_hint_fracs, double timing_advance, double delay_correction)
      : gr::block("txtime_setter",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(0, 0, 0)),
        d_fn_ref(init_fn),
        d_ts_ref(0),
        d_time_ref(init_time_secs,init_time_fracs),
        d_time_hint(time_hint_secs,time_hint_fracs),
        d_timing_advance(timing_advance),
        d_delay_correction(delay_correction)
    {
        message_port_register_in(pmt::intern("fn_time"));
        message_port_register_in(pmt::intern("bursts_in"));
        message_port_register_out(pmt::intern("bursts_out"));

        set_msg_handler(pmt::intern("fn_time"), boost::bind(&txtime_setter_impl::process_fn_time_reference, this, _1));
        set_msg_handler(pmt::intern("bursts_in"),  boost::bind(&txtime_setter_impl::process_txtime_of_burst,   this, _1));
    }

    /*
     * Our virtual destructor.
     */
    txtime_setter_impl::~txtime_setter_impl()
    {
    }
    
    void txtime_setter_impl::process_fn_time_reference(pmt::pmt_t msg)
    {
//      d_time_hint = pmt::to_python(pmt::dict_ref(msg, pmt::intern("time_hint"), pmt::PMT_NIL))
        pmt::pmt_t not_found = pmt::intern("not_found");
        pmt::pmt_t fn_time = pmt::dict_ref(msg, pmt::intern("fn_time"), not_found);
        pmt::pmt_t time_hint = pmt::dict_ref(msg, pmt::intern("fn_time"), not_found);

        if(fn_time != not_found)
        {
          uint32_t fn_ref = static_cast<uint32_t>(pmt::to_uint64(pmt::car(pmt::car(fn_time))));
          uint32_t ts =  static_cast<uint32_t>(pmt::to_uint64(pmt::cdr(pmt::car(fn_time))));
          uint64_t time_secs = pmt::to_uint64(pmt::car(pmt::cdr(fn_time)));
          double time_fracs = pmt::to_double(pmt::cdr(pmt::cdr(fn_time)));
          set_fn_time_reference(fn_ref, ts,  time_secs, time_fracs);
        } else if(time_hint != not_found) {
          set_time_hint(pmt::to_uint64(pmt::car(fn_time)), pmt::to_double(pmt::cdr(fn_time)));
        }

    }
    
    void txtime_setter_impl::process_txtime_of_burst(pmt::pmt_t burst)
    {
        pmt::pmt_t header_plus_burst = pmt::cdr(burst);
        int8_t * burst_int8 = (int8_t *)pmt::blob_data(header_plus_burst);        
        gsmtap_hdr * header = (gsmtap_hdr *)(burst_int8);
        
        uint32_t frame_nr = be32toh(header->frame_number); 
        uint32_t ts_num = header->timeslot;
        
        if(d_fn_ref != 0xffffffff)
        {
          time_format txtime = fn_time_delta_cpp(d_fn_ref, d_time_ref, frame_nr, d_time_hint, ts_num, d_ts_ref);
          time_spec_t txtime_spec = time_spec_t(txtime.first, txtime.second);
          txtime_spec = txtime_spec - d_delay_correction;
          txtime_spec = txtime_spec - d_timing_advance;
          
          if(txtime_spec <= time_spec_t(d_time_hint.first, d_time_hint.second))
          {
            std::cout << "lB" << std::flush;
          }
          else if(txtime_spec > time_spec_t(d_time_hint.first, d_time_hint.second)+10.0)
          {
            std::cout << "eB" << std::flush;
          }
          else
          {
            pmt::pmt_t tags_dict = pmt::dict_add(pmt::make_dict(), pmt::intern("tx_time"), pmt::make_tuple(pmt::from_uint64(txtime_spec.get_full_secs()),pmt::from_double(txtime_spec.get_frac_secs())));
            tags_dict = pmt::dict_add(tags_dict, pmt::intern("fn"), pmt::from_uint64(frame_nr));
            tags_dict = pmt::dict_add(tags_dict, pmt::intern("ts"), pmt::from_uint64(ts_num));
            pmt::pmt_t new_msg = pmt::cons(tags_dict, pmt::cdr(burst));
            message_port_pub(pmt::intern("bursts_out"), new_msg);
          }
        }
    }

    void txtime_setter_impl::set_fn_time_reference(uint32_t fn, uint32_t ts, uint64_t time_secs, double time_fracs)
    {
      d_fn_ref = fn;
      d_ts_ref = ts;
      d_time_ref = std::make_pair(time_secs, time_fracs);
      set_time_hint(time_secs, time_fracs);
    }

    void txtime_setter_impl::set_time_hint(uint64_t time_hint_secs, double time_hint_fracs)
    {
      d_time_hint = std::make_pair(time_hint_secs, time_hint_fracs);
    }

    void txtime_setter_impl::set_delay_correction(double delay_correction)
    {
      d_delay_correction = delay_correction;
    }

    void txtime_setter_impl::set_timing_advance(double timing_advance)
    {
      d_timing_advance = timing_advance;
    }

  } /* namespace gsm */
} /* namespace gr */

