/* -*- c++ -*- */
/*
 * Copyright 2017-2024 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <math.h>
#include "time_sample_ref.h"

namespace gr {
  namespace gsm {
    time_sample_ref::time_sample_ref(double samp_rate): d_samp_rate(samp_rate)
    {
    }

    time_sample_ref::~time_sample_ref()
    {
    }

    void time_sample_ref::update(time_spec_t last_rx_time, uint64_t current_start_offset)
    {
        d_last_rx_time = last_rx_time;
        d_current_start_offset = current_start_offset;
    }

    time_spec_t time_sample_ref::offset_to_time(uint64_t offset)
    {
      uint64_t samples_from_last_rx_time = offset - d_current_start_offset;
      time_spec_t time = time_spec_t(static_cast<double>(samples_from_last_rx_time)/d_samp_rate) + d_last_rx_time;

      return time;
    }

    uint64_t time_sample_ref::time_to_offset(time_spec_t time)
    {
      double samples_since_last_rx_time_tag = (time-d_last_rx_time).get_real_secs()*d_samp_rate;
//      double fractional_part = round(samples_since_last_rx_time_tag) - samples_since_last_rx_time_tag;
      uint64_t offset = static_cast<uint64_t>(round(samples_since_last_rx_time_tag)) + d_current_start_offset;

      return offset;
    }
  } // namespace gsm
} // namespace gr

