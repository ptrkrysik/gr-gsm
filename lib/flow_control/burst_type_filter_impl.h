/* -*- c++ -*- */
/*
 * Copyright 2017 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TYPE_FILTER_IMPL_H
#define INCLUDED_GSM_BURST_TYPE_FILTER_IMPL_H

#define BURST_TYPE_LEN 148

#include <gnuradio/gsm/flow_control/burst_type_filter.h>

namespace gr {
namespace gsm {

class burst_type_filter_impl : public burst_type_filter
{
private:
    filter_policy d_filter_policy;
    std::vector<uint8_t> d_selected_burst_types;

public:
    burst_type_filter_impl(const std::vector<uint8_t>& selected_burst_types);
    ~burst_type_filter_impl();
    void process_burst(pmt::pmt_t msg);

    /* External API */
    /* Filtering policy */
    filter_policy get_policy(void);
    filter_policy set_policy(filter_policy policy);

    void set_selected_burst_types(const std::vector<uint8_t>& selected_burst_types);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TYPE_FILTER_IMPL_H */
