/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "burst_timeslot_splitter_impl.h"
#include <gnuradio/gsm/endian.h>
#include <gnuradio/gsm/gsmtap.h>
#include <gnuradio/io_signature.h>
#include <stdio.h>

namespace gr {
namespace gsm {

burst_timeslot_splitter::sptr burst_timeslot_splitter::make()
{
    return gnuradio::get_initial_sptr(new burst_timeslot_splitter_impl());
}

/*
 * The private constructor
 */
burst_timeslot_splitter_impl::burst_timeslot_splitter_impl()
    : gr::block("burst_timeslot_splitter",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    message_port_register_in(pmt::mp("in"));

    message_port_register_out(pmt::mp("out0"));
    message_port_register_out(pmt::mp("out1"));
    message_port_register_out(pmt::mp("out2"));
    message_port_register_out(pmt::mp("out3"));
    message_port_register_out(pmt::mp("out4"));
    message_port_register_out(pmt::mp("out5"));
    message_port_register_out(pmt::mp("out6"));
    message_port_register_out(pmt::mp("out7"));

    set_msg_handler(pmt::mp("in"), boost::bind(&burst_timeslot_splitter_impl::process_burst, this, boost::placeholders::_1));
}

/*
 * Our virtual destructor.
 */
burst_timeslot_splitter_impl::~burst_timeslot_splitter_impl() {}

void burst_timeslot_splitter_impl::process_burst(pmt::pmt_t msg)
{
    pmt::pmt_t header_plus_burst = pmt::cdr(msg);
    gsmtap_hdr* header = (gsmtap_hdr*)pmt::blob_data(header_plus_burst);

    unsigned int timeslot = header->timeslot;

    std::string port("out");

    switch (timeslot) {
    case 0:
        port.append("0");
        break;
    case 1:
        port.append("1");
        break;
    case 2:
        port.append("2");
        break;
    case 3:
        port.append("3");
        break;
    case 4:
        port.append("4");
        break;
    case 5:
        port.append("5");
        break;
    case 6:
        port.append("6");
        break;
    case 7:
        port.append("7");
        break;
    default:
        port.append("0");
        break;
    }

    message_port_pub(pmt::mp(port), msg);
}
} /* namespace gsm */
} /* namespace gr */
