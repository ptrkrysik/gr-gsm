/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "burst_timeslot_filter_impl.h"
#include <gnuradio/gsm/endian.h>
#include <gnuradio/gsm/gsmtap.h>
#include <gnuradio/io_signature.h>
#include <stdio.h>

namespace gr {
namespace gsm {

burst_timeslot_filter::sptr burst_timeslot_filter::make(unsigned int timeslot)
{
    return gnuradio::get_initial_sptr(new burst_timeslot_filter_impl(timeslot));
}

/*
 * The private constructor
 */
burst_timeslot_filter_impl::burst_timeslot_filter_impl(unsigned int timeslot)
    : gr::block("burst_timeslot_filter",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)),
      d_timeslot(timeslot),
      d_filter_policy(FILTER_POLICY_DEFAULT)
{
    message_port_register_in(pmt::mp("in"));
    message_port_register_out(pmt::mp("out"));

    set_msg_handler(pmt::mp("in"), boost::bind(&burst_timeslot_filter_impl::process_burst, this, boost::placeholders::_1));
}

/*
 * Our virtual destructor.
 */
burst_timeslot_filter_impl::~burst_timeslot_filter_impl() {}

void burst_timeslot_filter_impl::process_burst(pmt::pmt_t msg)
{
    if (d_filter_policy == FILTER_POLICY_DROP_ALL)
        return;

    if (d_filter_policy == FILTER_POLICY_PASS_ALL) {
        message_port_pub(pmt::mp("out"), msg);
        return;
    }

    pmt::pmt_t header_plus_burst = pmt::cdr(msg);
    gsmtap_hdr* header = (gsmtap_hdr*)pmt::blob_data(header_plus_burst);

    unsigned int timeslot = header->timeslot;

    if (header->timeslot == d_timeslot) {
        message_port_pub(pmt::mp("out"), msg);
    }
}

/*
 * External API
 */
unsigned int burst_timeslot_filter_impl::get_tn(void) { return d_timeslot; }

unsigned int burst_timeslot_filter_impl::set_tn(unsigned int tn)
{
    if (tn < 8)
        d_timeslot = tn;

    return d_timeslot;
}

/* Filtering policy */
filter_policy burst_timeslot_filter_impl::get_policy(void) { return d_filter_policy; }

filter_policy burst_timeslot_filter_impl::set_policy(filter_policy policy)
{
    d_filter_policy = policy;
    return d_filter_policy;
}

} /* namespace gsm */
} /* namespace gr */
