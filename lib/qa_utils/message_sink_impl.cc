/* -*- c++ -*- */
/*
 * Copyright 2015 Roman Khassraf <rkhassraf@gmail.com>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "message_sink_impl.h"
#include <gnuradio/io_signature.h>
#include <stdio.h>
#include <iomanip>
#include <sstream>

namespace gr {
namespace gsm {

message_sink::sptr message_sink::make() { return gnuradio::get_initial_sptr(new message_sink_impl()); }

/*
 * The private constructor
 */
message_sink_impl::message_sink_impl()
    : gr::block("message_sink", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0))
{
    message_port_register_in(pmt::mp("in"));
    set_msg_handler(pmt::mp("in"), boost::bind(&message_sink_impl::process_message, this, boost::placeholders::_1));
}

/*
 * Our virtual destructor.
 */
message_sink_impl::~message_sink_impl()
{
    for (int i = 0; i < d_messages.size(); i++) {
        std::cout << d_messages[i].c_str() << std::endl;
    }
}

void message_sink_impl::process_message(pmt::pmt_t msg)
{
    pmt::pmt_t message_plus_header_blob = pmt::cdr(msg);
    uint8_t* message_plus_header = (uint8_t*)pmt::blob_data(message_plus_header_blob);
    size_t message_plus_header_len = pmt::blob_length(message_plus_header_blob);

    std::stringstream s_msg_stream;
    for (int i = 0; i < message_plus_header_len; i++) {
        if (i > 0) {
            s_msg_stream << (" ");
        }
        s_msg_stream << std::hex << std::setw(2) << std::setfill('0') << (unsigned)message_plus_header[i];
    }
    d_messages.push_back(s_msg_stream.str());
}

std::vector<std::string> message_sink_impl::get_messages() { return d_messages; }

} /* namespace gsm */
} /* namespace gr */
