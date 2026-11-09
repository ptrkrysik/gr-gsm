/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TIMESLOT_FILTER_IMPL_H
#define INCLUDED_GSM_BURST_TIMESLOT_FILTER_IMPL_H

#include <gnuradio/gsm/flow_control/burst_timeslot_filter.h>

namespace gr {
namespace gsm {

class burst_timeslot_filter_impl : public burst_timeslot_filter
{
private:
    filter_policy d_filter_policy;
    unsigned int d_timeslot;

public:
    burst_timeslot_filter_impl(unsigned int timeslot);
    ~burst_timeslot_filter_impl();
    void process_burst(pmt::pmt_t msg);

    /* External API */
    unsigned int get_tn(void);
    unsigned int set_tn(unsigned int tn);

    /* Filtering policy */
    filter_policy get_policy(void);
    filter_policy set_policy(filter_policy policy);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TIMESLOT_FILTER_IMPL_H */
