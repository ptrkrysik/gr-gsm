/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_DUMMY_BURST_FILTER_IMPL_H
#define INCLUDED_GSM_DUMMY_BURST_FILTER_IMPL_H

#define DUMMY_BURST_LEN 148

#include <gnuradio/gsm/flow_control/dummy_burst_filter.h>

namespace gr {
namespace gsm {

class dummy_burst_filter_impl : public dummy_burst_filter
{
private:
    bool is_dummy_burst(int8_t* burst, size_t burst_len);
    static const int8_t d_dummy_burst[];
    filter_policy d_filter_policy;

public:
    dummy_burst_filter_impl();
    ~dummy_burst_filter_impl();
    void process_burst(pmt::pmt_t msg);

    /* External API */
    /* Filtering policy */
    filter_policy get_policy(void);
    filter_policy set_policy(filter_policy policy);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_DUMMY_BURST_FILTER_IMPL_H */
