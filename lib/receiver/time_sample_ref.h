/* -*- c++ -*- */
/*
 * Copyright 2017-2024 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_TIME_SAMPLE_REF_IMPL_H
#define INCLUDED_TIME_SAMPLE_REF_IMPL_H

#include <stdint.h>
#include <gnuradio/gsm/misc_utils/time_spec.h>

namespace gr {
  namespace gsm {
    /*
    Class for storing time reference and for conversions time<->sample number 
    */
    class time_sample_ref
    {
     private:
        double d_samp_rate;
        time_spec_t d_last_rx_time;
        uint64_t d_current_start_offset;
     public:
        time_sample_ref(double samp_rate);
        ~time_sample_ref();
        void update(time_spec_t last_rx_time, uint64_t current_start_offset);
        time_spec_t offset_to_time(uint64_t offset);
        uint64_t time_to_offset(time_spec_t time);
    };
  } // namespace gsm
} // namespace gr
#endif// INCLUDED_TIME_SAMPLE_REF_IMPL_H
