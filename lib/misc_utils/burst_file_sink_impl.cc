/* -*- c++ -*- */
/*
 * Copyright 2015 by Roman Khassraf <rkhassraf@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "burst_file_sink_impl.h"
#include "stdio.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace gsm {

burst_file_sink::sptr burst_file_sink::make(const std::string& filename)
{
    return gnuradio::get_initial_sptr(new burst_file_sink_impl(filename));
}

/*
 * The private constructor
 */
burst_file_sink_impl::burst_file_sink_impl(const std::string& filename)
    : gr::block("burst_file_sink", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_output_file(filename.c_str(), std::ofstream::binary)
{
    message_port_register_in(pmt::mp("in"));
    set_msg_handler(pmt::mp("in"), boost::bind(&burst_file_sink_impl::process_burst, this, boost::placeholders::_1));
}

/*
 * Our virtual destructor.
 */
burst_file_sink_impl::~burst_file_sink_impl()
{
    if (d_output_file.is_open()) {
        d_output_file.close();
    }
}

void burst_file_sink_impl::process_burst(pmt::pmt_t msg)
{
    std::string s = pmt::serialize_str(msg);
    const char* serialized = s.data();
    d_output_file.write(serialized, s.length());
}
} /* namespace gsm */
} /* namespace gr */
