/* -*- c++ -*- */
/*
 * @file
 * @author (C) 2017 by Piotr Krysik <ptrkrysik@gmail.com>
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

#ifndef INCLUDED_TIME_SAMPLE_REF_IMPL_H
#define INCLUDED_TIME_SAMPLE_REF_IMPL_H

#include <stdint.h>
#include <grgsm/misc_utils/time_spec.h>

namespace gr {
  namespace gsm {
    /*
    Class for storing time reference and for conversions time<->sample number.
    N - number of sample before the resampler
    M - number of sample after the resampler
    t - time before the resampler
    */
    class time_sample_ref
    {
     private:
      double d_samp_rate;
      double d_resamp_rate;

      time_spec_t d_ref_time;
      uint64_t d_ref_offset;/**< sample offset of the reference point */
      uint64_t d_original_ref_offset;/**< sample offset of the
                                          reference point before resampler */
     public:
      time_sample_ref(double samp_rate, double resamp_rate = 1.0);
      ~time_sample_ref();

      void update(uint64_t M, uint64_t N){
        d_ref_time = time_spec_t::from_ticks(N - d_original_ref_offset, d_samp_rate*d_resamp_rate) + d_ref_time;
        d_original_ref_offset = N;
        d_ref_offset = M;
      }

      void update(uint64_t M, uint64_t N, double resamp_rate){
//        std::cout << "M:" << M << " N:" << N << " resamp_rate:" << resamp_rate << std::endl;
        update(M, N);
        d_resamp_rate = resamp_rate;
      }
      void update(uint64_t M, uint64_t N, time_spec_t ref_time){
//        std::cout << "M:" << M << " N:" << N << " ref_time" << ref_time.get_real_secs() << std::endl;
        update(M, N);
        d_ref_time = ref_time;
      }

      void set_samp_rate(double samp_rate){
        d_samp_rate = samp_rate;
      }

      uint64_t convert_M_to_N(uint64_t M){
        time_spec_t new_N_tspec = 
          time_spec_t::from_ticks(M - d_ref_offset, 1.0/d_resamp_rate) +
          time_spec_t::from_ticks(d_original_ref_offset, 1.0);
        uint64_t new_N = (round(new_N_tspec.get_real_secs()));
//        std::cout << "d_ref_offset:" << d_ref_offset << " d_resamp_rate:" << d_resamp_rate << " d_ref_offset" << d_ref_offset << " d_original_ref_offset:" << d_original_ref_offset << std::endl;
        return new_N;
      }

      time_spec_t convert_M_to_t(uint64_t M){
        return time_spec_t::from_ticks(convert_M_to_N(M) -
          d_original_ref_offset, d_samp_rate*d_resamp_rate) + d_ref_time;
      }

      time_spec_t convert_M_to_ideal_t(uint64_t M){
        return time_spec_t::from_ticks(M, d_samp_rate);
      }

      time_spec_t get_ref_time(){
        return d_ref_time;
      }

      uint64_t convert_t_to_M(time_spec_t time)
      {
        uint64_t samples_since_ref_time_tag = (time-d_ref_time).to_ticks(d_samp_rate);
        uint64_t offset = samples_since_ref_time_tag + d_ref_offset;
        return offset;
      }

      uint64_t convert_t_to_N(time_spec_t time)
      {
        return convert_M_to_N(convert_t_to_M(time));
      }
    };
  } // namespace gsm
} // namespace gr
#endif// INCLUDED_TIME_SAMPLE_REF_IMPL_H
