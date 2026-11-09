/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_GSM_BURST_TIMESLOT_SPLITTER_IMPL_H
#define INCLUDED_GSM_BURST_TIMESLOT_SPLITTER_IMPL_H

#include <gnuradio/gsm/flow_control/burst_timeslot_splitter.h>

namespace gr {
namespace gsm {

class burst_timeslot_splitter_impl : public burst_timeslot_splitter
{
public:
    burst_timeslot_splitter_impl();
    ~burst_timeslot_splitter_impl();
    void process_burst(pmt::pmt_t msg);
};

} // namespace gsm
} // namespace gr

#endif /* INCLUDED_GSM_BURST_TIMESLOT_SPLITTER_IMPL_H */
