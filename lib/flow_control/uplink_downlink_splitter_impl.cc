/* -*- c++ -*- */
/*
 * Copyright 2016 Piotr Krysik <ptrkrysik@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "uplink_downlink_splitter_impl.h"
#include <gnuradio/gsm/endian.h>
#include <gnuradio/gsm/gsmtap.h>
#include <gnuradio/io_signature.h>
#define BURST_SIZE 148
namespace gr {
namespace gsm {

uplink_downlink_splitter::sptr uplink_downlink_splitter::make()
{
    return gnuradio::get_initial_sptr(new uplink_downlink_splitter_impl());
}

/*
 * The private constructor
 */
uplink_downlink_splitter_impl::uplink_downlink_splitter_impl()
    : gr::block("uplink_downlink_splitter",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    message_port_register_in(pmt::mp("in"));
    message_port_register_out(pmt::mp("uplink"));
    message_port_register_out(pmt::mp("downlink"));
    set_msg_handler(pmt::mp("in"), boost::bind(&uplink_downlink_splitter_impl::process_msg, this, boost::placeholders::_1));
}

void uplink_downlink_splitter_impl::process_msg(pmt::pmt_t msg)
{
    gsmtap_hdr* header = (gsmtap_hdr*)(pmt::blob_data(pmt::cdr(msg)));
    bool uplink_burst = (be16toh(header->arfcn) & 0x4000) ? true : false;
    if (uplink_burst) {
        message_port_pub(pmt::mp("uplink"), msg);
    } else {
        message_port_pub(pmt::mp("downlink"), msg);
    }
}

/*
 * Our virtual destructor.
 */
uplink_downlink_splitter_impl::~uplink_downlink_splitter_impl() {}
} /* namespace gsm */
} /* namespace gr */
