/* -*- c++ -*- */
/*
 * Copyright 2015 by Roman Khassraf <rkhassraf@gmail.com>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "burst_file_source_impl.h"
#include "stdio.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace gsm {

burst_file_source::sptr burst_file_source::make(const std::string& filename)
{
    return gnuradio::get_initial_sptr(new burst_file_source_impl(filename));
}

/*
 * The private constructor
 */
burst_file_source_impl::burst_file_source_impl(const std::string& filename)
    : gr::block("burst_file_source", gr::io_signature::make(0, 0, 0), gr::io_signature::make(0, 0, 0)),
      d_input_file(filename.c_str(), std::ifstream::binary),
      d_finished(false)
{
    message_port_register_out(pmt::mp("out"));
}

/*
 * Our virtual destructor.
 */
burst_file_source_impl::~burst_file_source_impl()
{
    if (d_finished == false) {
        d_finished = true;
    }
}

bool burst_file_source_impl::start()
{
    d_finished = false;
    d_thread =
        std::shared_ptr<gr::thread::thread>(new gr::thread::thread(boost::bind(&burst_file_source_impl::run, this)));
    return block::start();
}

bool burst_file_source_impl::stop()
{
    d_finished = true;
    d_thread->interrupt();
    d_thread->join();
    return block::stop();
}

bool burst_file_source_impl::finished() { return d_finished; }

void burst_file_source_impl::run()
{
    std::filebuf* pbuf = d_input_file.rdbuf();
    while (!d_finished) {
        pmt::pmt_t burst = pmt::deserialize(*pbuf);
        if (pmt::is_eof_object(burst)) {
            break;
        }
        message_port_pub(pmt::mp("out"), burst);
    }
    d_input_file.close();
    post(pmt::mp("system"), pmt::cons(pmt::mp("done"), pmt::from_long(1)));
}
} /* namespace gsm */
} /* namespace gr */
