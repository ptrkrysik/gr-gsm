/* -*- c++ -*- */
/*
 * Copyright 2017 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_TXTIME_SETTER_IMPL_H
#define INCLUDED_GSM_TXTIME_SETTER_IMPL_H

#include <gnuradio/gsm/transmitter/txtime_setter.h>
#include <gnuradio/gsm/misc_utils/time_spec.h>
#include <gnuradio/gsm/misc_utils/fn_time.h>

namespace gr {
namespace gsm {

class txtime_setter_impl : public txtime_setter
{
private:
    uint32_t d_fn_ref;
    uint32_t d_ts_ref;
    time_format d_time_ref;
    time_format d_time_hint;
    double d_timing_advance;
    double d_delay_correction;

    void process_fn_time_reference(pmt::pmt_t msg);
    void process_txtime_of_burst(pmt::pmt_t msg);

public:
    txtime_setter_impl(uint32_t init_fn, uint64_t init_time_secs,
        double init_time_fracs, uint64_t time_hint_secs,
        double time_hint_fracs, double timing_advance,
        double delay_correction);
    ~txtime_setter_impl();

    // Where all the action really happens
    void set_fn_time_reference(uint32_t fn, uint32_t ts,
        uint64_t time_secs, double time_fracs);
    void set_time_hint(uint64_t time_hint_secs, double time_hint_fracs);
    void set_delay_correction(double delay_correction);
    void set_timing_advance(double timing_advance);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_TXTIME_SETTER_IMPL_H */
