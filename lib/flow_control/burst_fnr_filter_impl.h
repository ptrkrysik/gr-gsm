/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_FNR_FILTER_IMPL_H
#define INCLUDED_GSM_BURST_FNR_FILTER_IMPL_H

#include <gnuradio/gsm/flow_control/burst_fnr_filter.h>

#define GSM_SUPERFRAME (26 * 51)
#define GSM_HYPERFRAME (2048 * GSM_SUPERFRAME)

namespace gr {
namespace gsm {

class burst_fnr_filter_impl : public burst_fnr_filter
{
private:
    filter_policy d_filter_policy;
    unsigned int d_framenr;
    filter_mode d_mode;

public:
    burst_fnr_filter_impl(filter_mode mode, unsigned int fnr);
    ~burst_fnr_filter_impl();
    void process_burst(pmt::pmt_t msg);

    /* External API */
    unsigned int get_fn(void);
    unsigned int set_fn(unsigned int fn);

    filter_mode get_mode(void);
    filter_mode set_mode(filter_mode mode);

    /* Filtering policy */
    filter_policy get_policy(void);
    filter_policy set_policy(filter_policy policy);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_FNR_FILTER_IMPL_H */
