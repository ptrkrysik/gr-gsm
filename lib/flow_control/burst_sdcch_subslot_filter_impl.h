/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_IMPL_H
#define INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_IMPL_H

#include <gnuradio/gsm/flow_control/burst_sdcch_subslot_filter.h>

namespace gr {
namespace gsm {

class burst_sdcch_subslot_filter_impl : public burst_sdcch_subslot_filter
{
private:
    filter_policy d_filter_policy;
    subslot_filter_mode d_mode;
    unsigned int d_subslot;

public:
    burst_sdcch_subslot_filter_impl(subslot_filter_mode mode, unsigned int subslot);
    ~burst_sdcch_subslot_filter_impl();
    void process_burst(pmt::pmt_t msg);

    /* External API */
    unsigned int get_ss(void);
    unsigned int set_ss(unsigned int ss);

    subslot_filter_mode get_mode(void);
    subslot_filter_mode set_mode(subslot_filter_mode mode);

    /* Filtering policy */
    filter_policy get_policy(void);
    filter_policy set_policy(filter_policy policy);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_SDCCH_SUBSLOT_FILTER_IMPL_H */
