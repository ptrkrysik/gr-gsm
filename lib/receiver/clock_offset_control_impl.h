/* -*- c++ -*- */
/*
 * Copyright 2015-2017 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_CLOCK_OFFSET_CONTROL_IMPL_H
#define INCLUDED_GSM_CLOCK_OFFSET_CONTROL_IMPL_H

#include <gnuradio/gsm/receiver/clock_offset_control.h>
#include <string>

namespace gr {
namespace gsm {
class clock_offset_control_impl : public clock_offset_control
{
private:
    float d_fc;
    float d_samp_rate;
    unsigned int d_osr;
    float d_alfa;
    float d_ppm_estimate;
    float d_last_ppm_estimate;
    bool d_first_measurement;
    int d_counter;
    std::string d_last_state;
    float d_current_time;
    float d_last_fcch_time;
    bool d_first_time;

    void process_measurement(pmt::pmt_t msg);
    void send_ctrl_messages(float freq_offset);
    void timed_reset();
    void reset();

public:
    clock_offset_control_impl(float fc, float samp_rate, unsigned int osr);
    ~clock_offset_control_impl();

    virtual void set_fc(float fc);
    virtual void set_samp_rate(float samp_rate);
    virtual void set_osr(unsigned int osr);
};
} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_CLOCK_OFFSET_CONTROL_IMPL_H */
